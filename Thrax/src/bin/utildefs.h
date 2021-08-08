// Definitions needed by various utilities here.

#ifndef NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_
#define NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_

#include <string>
#include <utility>
#include <vector>

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <fst/arc.h>
#include <fst/fst.h>
#include <fst/symbol-table.h>
#include <fst/vector-fst.h>
#include <thrax/grm-manager.h>

namespace thrax {

// Computes the n-shortest paths and returns a vector of strings, each string
// corresponding to each path. The mapping of labels to strings is controlled by
// the type and the symtab. Elements that are in the generated label set from
// the grammar are output as "[name]" where "name" is the name of the generated
// label. Paths are sorted in ascending order of weights.

bool FstToStrings(const ::fst::VectorFst<::fst::StdArc> &fst,
                  std::vector<std::pair<std::string, float>> *strings,
                  const ::fst::SymbolTable *generated_symtab,
                  ::fst::TokenType type = ::fst::TokenType::BYTE,
                  ::fst::SymbolTable *symtab = nullptr, size_t n = 1);

// Find the generated labels from the grammar.

const ::fst::SymbolTable *GetGeneratedSymbolTable(
    GrmManagerSpec<::fst::StdArc> *grm);

}  // namespace thrax

#endif  // NLP_GRM_LANGUAGE_UTIL_UTILDEFS_H_
