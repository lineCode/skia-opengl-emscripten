// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Generated from template:
//   core/css/properties/templates/css_property_subclass.h.tmpl
// and input files:
//   ../../third_party/blink/renderer/core/css/computed_style_field_aliases.json5
//   ../../third_party/blink/renderer/core/css/css_properties.json5
//   ../../third_party/blink/renderer/core/css/properties/css_property_methods.json5


#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_SHORTHAND_BORDER_BLOCK_END_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_SHORTHAND_BORDER_BLOCK_END_H_

#include "third_party/blink/renderer/core/css/css_identifier_value.h"
#include "third_party/blink/renderer/core/css/css_primitive_value_mappings.h"
#include "third_party/blink/renderer/core/css/properties/shorthand.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver_state.h"
#include "third_party/blink/renderer/core/style/computed_style.h"
#include "third_party/blink/renderer/core/style_property_shorthand.h"

namespace blink {
namespace css_shorthand {

// Implements the 'border-block-end' CSS property
// See src/third_party/blink/renderer/core/css/properties/README.md
class BorderBlockEnd final : public Shorthand {
 public:
  constexpr BorderBlockEnd() : Shorthand(CSSPropertyID::kBorderBlockEnd,
    0
    | kProperty
    ,
    '\0'
    ) {}

  const char* GetPropertyName() const override { return "border-block-end"; }
  const WTF::AtomicString& GetPropertyNameAtomicString() const override {
    DEFINE_STATIC_LOCAL(const AtomicString, name, ("border-block-end"));
    return name;
  }
  const char* GetJSPropertyName() const override {
    return "borderBlockEnd";
  }
  bool ParseShorthand(bool, CSSParserTokenRange&, const CSSParserContext&, const CSSParserLocalContext&, HeapVector<CSSPropertyValue, 256>&) const override;
  const CSSValue* CSSValueFromComputedStyleInternal(
      const ComputedStyle& ,
      const SVGComputedStyle&,
      const LayoutObject*,
      Node*,
      bool allow_visited_style) const override {
    // Directional properties are resolved by resolveDirectionAwareProperty()
    // before calling CSSValueFromComputedStyleInternal.
    NOTREACHED();
    return nullptr;
  }
  const CSSProperty& ResolveDirectionAwareProperty(
      TextDirection direction,
      blink::WritingMode writing_mode) const override {
    return ResolveAfterToPhysicalProperty(
      direction,
      writing_mode,
      BorderDirections());
  }
};

}  // namespace css_shorthand
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_SHORTHAND_BORDER_BLOCK_END_H_
