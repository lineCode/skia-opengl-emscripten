// Copyright 2014 The Cobalt Authors. All Rights Reserved.
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

#ifndef COBALT_DOM_COMMENT_H_
#define COBALT_DOM_COMMENT_H_

#include <string>

#include "base/strings/string_piece.h"
#include "cobalt/dom/character_data.h"
#include "cobalt/script/environment_settings.h"

namespace cobalt {
namespace dom {

// The Comment interface represents textual notations within markup; although
// it is generally not visually shown, such comments can be still retrieved
// from the document.
//   https://www.w3.org/TR/2014/WD-dom-20140710/#interface-comment
class Comment : public CharacterData {
 public:
  Comment(script::EnvironmentSettings* env_settings,
          const base::StringPiece& comment);
  Comment(Document* document, const base::StringPiece& comment);

  // Web API: Node
  //
  base::CobToken node_name() const override;
  NodeType node_type() const override { return Node::kCommentNode; }

  // Custom, not in any spec: Node.
  //
  Comment* AsComment() override { return this; }

  void Accept(NodeVisitor* visitor) override;
  void Accept(ConstNodeVisitor* visitor) const override;

  scoped_refptr<Node> Duplicate() const override;

  DEFINE_WRAPPABLE_TYPE(Comment);

 private:
  ~Comment() override {}
};

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_COMMENT_H_
