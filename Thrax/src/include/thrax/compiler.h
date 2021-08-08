#ifndef NLP_GRM_LANGUAGE_COMPILER_H_
#define NLP_GRM_LANGUAGE_COMPILER_H_

#include <fst/compat.h>
#include <thrax/compat/compat.h>
#include <thrax/grm-compiler.h>
#include <thrax/grm-manager.h>

DECLARE_string(indir);

namespace thrax {

template <typename Arc>
bool CompileGrammar(const std::string& input_grammar,
                    const std::string& output_far, bool emit_ast_only,
                    bool line_numbers_in_ast) {
  GrmCompilerSpec<Arc> grammar;
  if (!grammar.ParseFile(JoinPath(FLAGS_indir, input_grammar))) {
    return false;
  }
  if (emit_ast_only) {
    return grammar.PrintAst(line_numbers_in_ast);
  } else if (grammar.EvaluateAst()) {
    const GrmManagerSpec<Arc>* manager = grammar.GetGrmManager();
    manager->ExportFar(output_far);
    return true;
  }
  return false;
}

extern template bool CompileGrammar<::fst::StdArc>(const std::string&,
                                                       const std::string&, bool,
                                                       bool);

extern template bool CompileGrammar<::fst::LogArc>(const std::string&,
                                                       const std::string&, bool,
                                                       bool);

extern template bool CompileGrammar<::fst::Log64Arc>(const std::string&,
                                                         const std::string&,
                                                         bool, bool);

}  // namespace thrax

#endif  // NLP_GRM_LANGUAGE_COMPILER_H_
