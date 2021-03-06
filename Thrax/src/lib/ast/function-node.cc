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
#include <thrax/function-node.h>

#include <memory>

#include <thrax/collection-node.h>
#include <thrax/identifier-node.h>
#include <thrax/node.h>
#include <thrax/walker.h>

namespace thrax {

FunctionNode::FunctionNode(IdentifierNode* name,
                           CollectionNode* arguments,
                           CollectionNode* body)
    : Node(), name_(name), arguments_(arguments), body_(body) {}

IdentifierNode* FunctionNode::GetName() const { return name_.get(); }

CollectionNode* FunctionNode::GetArguments() const { return arguments_.get(); }

CollectionNode* FunctionNode::GetBody() const { return body_.get(); }

void FunctionNode::Accept(AstWalker* walker) { walker->Visit(this); }

}  // namespace thrax
