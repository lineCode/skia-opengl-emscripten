// Copyright 2015 The Cobalt Authors. All Rights Reserved.
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

#ifndef COBALT_DOM_HTML_PARAGRAPH_ELEMENT_H_
#define COBALT_DOM_HTML_PARAGRAPH_ELEMENT_H_

#include <string>

#include "cobalt/dom/html_element.h"

namespace cobalt {
namespace dom {

class Document;

// The p element represents a paragraph.
//   https://www.w3.org/TR/html5/grouping-content.html#the-p-element
class HTMLParagraphElement : public HTMLElement {
 public:
  static const char kTagName[];

  explicit HTMLParagraphElement(Document* document)
      : HTMLElement(document, base::CobToken(kTagName)) {}

  // Custom, not in any spec.
  scoped_refptr<HTMLParagraphElement> AsHTMLParagraphElement() override {
    return this;
  }

  DEFINE_WRAPPABLE_TYPE(HTMLParagraphElement);

 private:
  ~HTMLParagraphElement() override {}
};

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_HTML_PARAGRAPH_ELEMENT_H_
