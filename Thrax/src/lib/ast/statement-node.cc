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
// Copyright 2005-2011 Google, Inc.
// Author: ttai@google.com (Terry Tai)

#include <thrax/statement-node.h>

#include <iostream>
#include <string>

#include <thrax/walker.h>

namespace thrax {

StatementNode::StatementNode(StatementNodeType type)
    : Node(), type_(type), statement_(NULL) {}

StatementNode::~StatementNode() {
  delete statement_;
}

StatementNode::StatementNodeType StatementNode::GetType() const {
  return type_;
}

void StatementNode::Set(Node* statement) {
  statement_ = statement;
}

Node* StatementNode::Get() const {
  return statement_;
}

void StatementNode::Accept(AstWalker* walker) {
  walker->Visit(this);
}

}  // namespace thrax
