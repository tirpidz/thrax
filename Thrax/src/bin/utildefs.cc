// Definitions needed by various utilities here.

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <../bin/utildefs.h>

#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <fst/arc.h>
#include <fst/fst.h>
#include <fst/string.h>
#include <fst/symbol-table.h>
#include <fst/vector-fst.h>
#include <thrax/algo/paths.h>
#include <thrax/grm-manager.h>

DEFINE_string(field_separator, " ",
              "Field separator for strings of symbols from a symbol table.");

namespace thrax {
namespace {

using ::fst::kNoStateId;
using ::fst::LabelsToUTF8String;
using ::fst::PathIterator;
using ::fst::Project;
using ::fst::PROJECT_OUTPUT;
using ::fst::RmEpsilon;
using ::fst::ShortestPath;
using ::fst::StdArc;
using ::fst::StdVectorFst;
using ::fst::SymbolTable;

using Label = StdArc::Label;

inline bool AppendLabel(Label label, TokenType type,
                        const SymbolTable *generated_symtab,
                        SymbolTable *symtab, std::string *path) {
  if (label != 0) {
    // Check first to see if this label is in the generated symbol set. Note
    // that this should not conflict with a user-provided symbol table since
    // the parser used by GrmCompiler doesn't generate extra labels if a
    // string is parsed using a user-provided symbol table.
    if (generated_symtab && !generated_symtab->Find(label).empty()) {
      const auto &sym = generated_symtab->Find(label);
      *path += "[" + sym + "]";
    } else if (type == SYMBOL) {
      const auto &sym = symtab->Find(label);
      if (sym.empty()) {
        LOG(ERROR) << "Missing symbol in symbol table for id: " << label;
        return false;
      }
      // For non-byte, non-UTF8 symbols, one overwhelmingly wants these to be
      // space-separated.
      if (!path->empty()) *path += FLAGS_field_separator;
      *path += sym;
    } else if (type == BYTE) {
      path->push_back(label);
    } else if (type == UTF8) {
      std::string utf8_string;
      std::vector<Label> labels;
      labels.push_back(label);
      if (!LabelsToUTF8String(labels, &utf8_string)) {
        LOG(ERROR) << "LabelsToUTF8String: Bad code point: " << label;
        return false;
      }
      *path += utf8_string;
    }
  }
  return true;
}

}  // namespace

bool FstToStrings(const StdVectorFst &fst,
                  std::vector<std::pair<std::string, float>> *strings,
                  const SymbolTable *generated_symtab, TokenType type,
                  SymbolTable *symtab, size_t n) {
  StdVectorFst shortest_path;
  if (n == 1) {
    ShortestPath(fst, &shortest_path, n);
  } else {
    // The uniqueness feature of ShortestPath requires us to have an acceptor,
    // so we project and remove epsilon arcs.
    StdVectorFst temp(fst);
    Project(&temp, PROJECT_OUTPUT);
    RmEpsilon(&temp);
    ShortestPath(temp, &shortest_path, n, /*unique=*/true);
  }
  if (shortest_path.Start() == kNoStateId) return false;
  for (PathIterator<StdArc> iter(shortest_path, /*check_acyclic=*/false);
       !iter.Done(); iter.Next()) {
    std::string path;
    for (const auto label : iter.OLabels()) {
      if (!AppendLabel(label, type, generated_symtab, symtab, &path)) {
        return false;
      }
    }
    strings->emplace_back(std::move(path), iter.Weight().Value());
  }
  return true;
}

const SymbolTable *GetGeneratedSymbolTable(GrmManagerSpec<StdArc> *grm) {
  const auto *symbolfst = grm->GetFst("*StringFstSymbolTable");
  return symbolfst ? symbolfst->InputSymbols()->Copy() : nullptr;
}

}  // namespace thrax
