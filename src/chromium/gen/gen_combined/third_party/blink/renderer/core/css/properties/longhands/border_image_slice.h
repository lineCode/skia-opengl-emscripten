// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Generated from template:
//   core/css/properties/templates/css_property_subclass.h.tmpl
// and input files:
//   ../../third_party/blink/renderer/core/css/computed_style_field_aliases.json5
//   ../../third_party/blink/renderer/core/css/css_properties.json5
//   ../../third_party/blink/renderer/core/css/properties/css_property_methods.json5


#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_LONGHAND_BORDER_IMAGE_SLICE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_LONGHAND_BORDER_IMAGE_SLICE_H_

#include "third_party/blink/renderer/core/css/css_identifier_value.h"
#include "third_party/blink/renderer/core/css/css_primitive_value_mappings.h"
#include "third_party/blink/renderer/core/css/properties/longhand.h"
#include "third_party/blink/renderer/core/css/properties/style_building_utils.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver_state.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {
namespace css_longhand {

// Implements the 'border-image-slice' CSS property
// See src/third_party/blink/renderer/core/css/properties/README.md
class BorderImageSlice final : public Longhand {
 public:
  constexpr BorderImageSlice() : Longhand(CSSPropertyID::kBorderImageSlice,
    0
    | kInterpolable
    | kProperty
    ,
    '\0'
    ) {}

  const char* GetPropertyName() const override { return "border-image-slice"; }
  const WTF::AtomicString& GetPropertyNameAtomicString() const override {
    DEFINE_STATIC_LOCAL(const AtomicString, name, ("border-image-slice"));
    return name;
  }
  const char* GetJSPropertyName() const override {
    return "borderImageSlice";
  }
  const CSSValue* ParseSingleValue(CSSParserTokenRange&, const CSSParserContext&, const CSSParserLocalContext&) const override;
  const CSSValue* CSSValueFromComputedStyleInternal(const ComputedStyle&, const SVGComputedStyle&, const LayoutObject*, Node*, bool allow_visited_style) const override;
  const CSSValue* InitialValue() const override;

  // Style builder functions
  void ApplyInitial(StyleResolverState& state) const override {
    const NinePieceImage& current_image = state.Style()->BorderImage();
    if (current_image.Fill() == false &&
        style_building_utils::LengthMatchesAllSides(current_image.ImageSlices(), Length::Percent(100)))
      return;
    NinePieceImage image(current_image);
    image.SetImageSlices(LengthBox(Length::Percent(100), Length::Percent(100), Length::Percent(100), Length::Percent(100)));
    image.SetFill(false);
    state.Style()->SetBorderImage(image);
  }
  void ApplyInherit(StyleResolverState& state) const override {
    NinePieceImage image(state.Style()->BorderImage());
    image.CopyImageSlicesFrom(state.ParentStyle()->BorderImage());
    state.Style()->SetBorderImage(image);
  }
  void ApplyValue(StyleResolverState& state, const CSSValue& value) const override {
    NinePieceImage image(state.Style()->BorderImage());
    CSSToStyleMap::MapNinePieceImageSlice(state, value, image);
    state.Style()->SetBorderImage(image);
  }
};

}  // namespace css_longhand
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PROPERTIES_LONGHAND_BORDER_IMAGE_SLICE_H_
