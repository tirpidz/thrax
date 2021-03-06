// Copyright 2005-2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <thrax/grm-compiler.h>

DEFINE_bool(always_export, false, "Export all rules (for debugging purposes)");
DEFINE_bool(print_ast, false, "Whether we print out the AST to stdout");
DEFINE_bool(line_numbers_in_ast, false, "Print line numbers in AST");
