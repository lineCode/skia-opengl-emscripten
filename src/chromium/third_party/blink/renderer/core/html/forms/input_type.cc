/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 *           (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
 * Copyright (C) 2009, 2010, 2011, 2012 Google Inc. All rights reserved.
 * Copyright (C) 2012 Samsung Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "third_party/blink/renderer/core/html/forms/input_type.h"

#include <limits>
#include <memory>
#include <utility>

#include "third_party/blink/renderer/core/accessibility/ax_object_cache.h"
#include "third_party/blink/renderer/core/dom/events/scoped_event_queue.h"
#include "third_party/blink/renderer/core/dom/node_computed_style.h"
#include "third_party/blink/renderer/core/events/keyboard_event.h"
#include "third_party/blink/renderer/core/fileapi/file_list.h"
#include "third_party/blink/renderer/core/html/forms/button_input_type.h"
#include "third_party/blink/renderer/core/html/forms/checkbox_input_type.h"
#include "third_party/blink/renderer/core/html/forms/color_chooser.h"
#include "third_party/blink/renderer/core/html/forms/color_input_type.h"
#include "third_party/blink/renderer/core/html/forms/date_input_type.h"
#include "third_party/blink/renderer/core/html/forms/date_time_local_input_type.h"
#include "third_party/blink/renderer/core/html/forms/email_input_type.h"
#include "third_party/blink/renderer/core/html/forms/file_input_type.h"
#include "third_party/blink/renderer/core/html/forms/form_data.h"
#include "third_party/blink/renderer/core/html/forms/hidden_input_type.h"
#include "third_party/blink/renderer/core/html/forms/html_form_element.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/forms/image_input_type.h"
#include "third_party/blink/renderer/core/html/forms/month_input_type.h"
#include "third_party/blink/renderer/core/html/forms/number_input_type.h"
#include "third_party/blink/renderer/core/html/forms/password_input_type.h"
#include "third_party/blink/renderer/core/html/forms/radio_input_type.h"
#include "third_party/blink/renderer/core/html/forms/range_input_type.h"
#include "third_party/blink/renderer/core/html/forms/reset_input_type.h"
#include "third_party/blink/renderer/core/html/forms/search_input_type.h"
#include "third_party/blink/renderer/core/html/forms/submit_input_type.h"
#include "third_party/blink/renderer/core/html/forms/telephone_input_type.h"
#include "third_party/blink/renderer/core/html/forms/text_input_type.h"
#include "third_party/blink/renderer/core/html/forms/time_input_type.h"
#include "third_party/blink/renderer/core/html/forms/url_input_type.h"
#include "third_party/blink/renderer/core/html/forms/week_input_type.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/input_type_names.h"
#include "third_party/blink/renderer/core/inspector/console_message.h"
#include "third_party/blink/renderer/core/layout/layout_theme.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/json/json_values.h"
#include "third_party/blink/renderer/platform/text/platform_locale.h"
#include "third_party/blink/renderer/platform/text/text_break_iterator.h"

namespace blink {

using blink::WebLocalizedString;
using namespace html_names;

using InputTypeFactoryFunction = InputType* (*)(HTMLInputElement&);
using InputTypeFactoryMap = HashMap<AtomicString, InputTypeFactoryFunction>;

static std::unique_ptr<InputTypeFactoryMap> CreateInputTypeFactoryMap() {
  std::unique_ptr<InputTypeFactoryMap> map =
      std::make_unique<InputTypeFactoryMap>();
  map->insert(input_type_names::kButton,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<ButtonInputType>(element);
              });
  map->insert(input_type_names::kCheckbox,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<CheckboxInputType>(element);
              });
  map->insert(input_type_names::kColor,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<ColorInputType>(element);
              });
  map->insert(input_type_names::kDate,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<DateInputType>(element);
              });
  map->insert(input_type_names::kDatetimeLocal,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<DateTimeLocalInputType>(element);
              });
  map->insert(input_type_names::kEmail,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<EmailInputType>(element);
              });
  map->insert(input_type_names::kFile,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<FileInputType>(element);
              });
  map->insert(input_type_names::kHidden,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<HiddenInputType>(element);
              });
  map->insert(input_type_names::kImage,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<ImageInputType>(element);
              });
  map->insert(input_type_names::kMonth,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<MonthInputType>(element);
              });
  map->insert(input_type_names::kNumber,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<NumberInputType>(element);
              });
  map->insert(input_type_names::kPassword,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<PasswordInputType>(element);
              });
  map->insert(input_type_names::kRadio,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<RadioInputType>(element);
              });
  map->insert(input_type_names::kRange,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<RangeInputType>(element);
              });
  map->insert(input_type_names::kReset,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<ResetInputType>(element);
              });
  map->insert(input_type_names::kSearch,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<SearchInputType>(element);
              });
  map->insert(input_type_names::kSubmit,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<SubmitInputType>(element);
              });
  map->insert(input_type_names::kTel,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<TelephoneInputType>(element);
              });
  map->insert(input_type_names::kTime,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<TimeInputType>(element);
              });
  map->insert(input_type_names::kUrl,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<URLInputType>(element);
              });
  map->insert(input_type_names::kWeek,
              [](HTMLInputElement& element) -> InputType* {
                return MakeGarbageCollected<WeekInputType>(element);
              });
  // No need to register "text" because it is the default type.
  return map;
}

static const InputTypeFactoryMap* FactoryMap() {
  static const InputTypeFactoryMap* factory_map =
      CreateInputTypeFactoryMap().release();
  return factory_map;
}

InputType* InputType::Create(HTMLInputElement& element,
                             const AtomicString& type_name) {
  InputTypeFactoryFunction factory =
      type_name.IsEmpty() ? nullptr : FactoryMap()->at(type_name);
  if (!factory)
    factory = TextInputType::Create;
  return factory(element);
}

InputType* InputType::CreateText(HTMLInputElement& element) {
  return TextInputType::Create(element);
}

const AtomicString& InputType::NormalizeTypeName(
    const AtomicString& type_name) {
  if (type_name.IsEmpty())
    return input_type_names::kText;
  InputTypeFactoryMap::const_iterator it =
      FactoryMap()->find(type_name.LowerASCII());
  return it == FactoryMap()->end() ? input_type_names::kText : it->key;
}

InputType::~InputType() = default;

void InputType::Trace(Visitor* visitor) {
  visitor->Trace(element_);
}

bool InputType::IsTextField() const {
  return false;
}

bool InputType::ShouldSaveAndRestoreFormControlState() const {
  return true;
}

bool InputType::IsFormDataAppendable() const {
  // There is no form data unless there's a name for non-image types.
  return !GetElement().GetName().IsEmpty();
}

void InputType::AppendToFormData(FormData& form_data) const {
  form_data.AppendFromElement(GetElement().GetName(), GetElement().value());
}

String InputType::ResultForDialogSubmit() const {
  return GetElement().FastGetAttribute(kValueAttr);
}

double InputType::ValueAsDate() const {
  return DateComponents::InvalidMilliseconds();
}

void InputType::SetValueAsDate(double, ExceptionState& exception_state) const {
  exception_state.ThrowDOMException(
      DOMExceptionCode::kInvalidStateError,
      "This input element does not support Date values.");
}

double InputType::ValueAsDouble() const {
  return std::numeric_limits<double>::quiet_NaN();
}

void InputType::SetValueAsDouble(double double_value,
                                 TextFieldEventBehavior event_behavior,
                                 ExceptionState& exception_state) const {
  exception_state.ThrowDOMException(
      DOMExceptionCode::kInvalidStateError,
      "This input element does not support Number values.");
}

void InputType::SetValueAsDecimal(const Decimal& new_value,
                                  TextFieldEventBehavior event_behavior,
                                  ExceptionState&) const {
  GetElement().setValue(Serialize(new_value), event_behavior);
}

void InputType::ReadingChecked() const {}

bool InputType::SupportsValidation() const {
  return true;
}

bool InputType::TypeMismatchFor(const String&) const {
  return false;
}

bool InputType::TypeMismatch() const {
  return false;
}

bool InputType::SupportsRequired() const {
  // Almost all validatable types support @required.
  return SupportsValidation();
}

bool InputType::ValueMissing(const String&) const {
  return false;
}

bool InputType::TooLong(const String&,
                        TextControlElement::NeedsToCheckDirtyFlag) const {
  return false;
}

bool InputType::TooShort(const String&,
                         TextControlElement::NeedsToCheckDirtyFlag) const {
  return false;
}

bool InputType::PatternMismatch(const String&) const {
  return false;
}

bool InputType::RangeUnderflow(const String& value) const {
  if (!IsSteppable())
    return false;

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  return numeric_value < CreateStepRange(kRejectAny).Minimum();
}

bool InputType::RangeOverflow(const String& value) const {
  if (!IsSteppable())
    return false;

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  return numeric_value > CreateStepRange(kRejectAny).Maximum();
}

Decimal InputType::DefaultValueForStepUp() const {
  return 0;
}

double InputType::Minimum() const {
  return CreateStepRange(kRejectAny).Minimum().ToDouble();
}

double InputType::Maximum() const {
  return CreateStepRange(kRejectAny).Maximum().ToDouble();
}

bool InputType::IsInRange(const String& value) const {
  if (!IsSteppable())
    return false;

  // This function should return true if both of validity.rangeUnderflow and
  // validity.rangeOverflow are false.
  // If the INPUT has no value, they are false.
  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return true;

  StepRange step_range(CreateStepRange(kRejectAny));
  return step_range.HasRangeLimitations() &&
         numeric_value >= step_range.Minimum() &&
         numeric_value <= step_range.Maximum();
}

bool InputType::IsOutOfRange(const String& value) const {
  if (!IsSteppable())
    return false;

  // This function should return true if either validity.rangeUnderflow or
  // validity.rangeOverflow are true.
  // If the INPUT has no value, they are false.
  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  StepRange step_range(CreateStepRange(kRejectAny));
  return step_range.HasRangeLimitations() &&
         (numeric_value < step_range.Minimum() ||
          numeric_value > step_range.Maximum());
}

void InputType::InRangeChanged() const {
  if (IsSteppable()) {
    GetElement().PseudoStateChanged(CSSSelector::kPseudoInRange);
    GetElement().PseudoStateChanged(CSSSelector::kPseudoOutOfRange);
  }
}

bool InputType::StepMismatch(const String& value) const {
  if (!IsSteppable())
    return false;

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  return CreateStepRange(kRejectAny).StepMismatch(numeric_value);
}

String InputType::BadInputText() const {
  NOTREACHED();
  return GetLocale().QueryString(WebLocalizedString::kValidationTypeMismatch);
}

String InputType::RangeOverflowText(const Decimal&) const {
  NOTREACHED();
  return String();
}

String InputType::RangeUnderflowText(const Decimal&) const {
  NOTREACHED();
  return String();
}

String InputType::TypeMismatchText() const {
  return GetLocale().QueryString(WebLocalizedString::kValidationTypeMismatch);
}

String InputType::ValueMissingText() const {
  return GetLocale().QueryString(WebLocalizedString::kValidationValueMissing);
}

std::pair<String, String> InputType::ValidationMessage(
    const InputTypeView& input_type_view) const {
  const String value = GetElement().value();

  // The order of the following checks is meaningful. e.g. We'd like to show the
  // badInput message even if the control has other validation errors.
  if (input_type_view.HasBadInput())
    return std::make_pair(BadInputText(), g_empty_string);

  if (ValueMissing(value))
    return std::make_pair(ValueMissingText(), g_empty_string);

  if (TypeMismatch())
    return std::make_pair(TypeMismatchText(), g_empty_string);

  if (PatternMismatch(value)) {
    // https://html.spec.whatwg.org/C/#attr-input-pattern
    //   When an input element has a pattern attribute specified, authors
    //   should include a title attribute to give a description of the
    //   pattern. User agents may use the contents of this attribute, if it
    //   is present, when informing the user that the pattern is not matched
    return std::make_pair(
        GetLocale().QueryString(WebLocalizedString::kValidationPatternMismatch),
        GetElement().FastGetAttribute(kTitleAttr).GetString());
  }

  if (GetElement().TooLong()) {
    return std::make_pair(GetLocale().ValidationMessageTooLongText(
                              value.length(), GetElement().maxLength()),
                          g_empty_string);
  }

  if (GetElement().TooShort()) {
    return std::make_pair(GetLocale().ValidationMessageTooShortText(
                              value.length(), GetElement().minLength()),
                          g_empty_string);
  }

  if (!IsSteppable())
    return std::make_pair(g_empty_string, g_empty_string);

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return std::make_pair(g_empty_string, g_empty_string);

  StepRange step_range(CreateStepRange(kRejectAny));

  if (numeric_value < step_range.Minimum())
    return std::make_pair(RangeUnderflowText(step_range.Minimum()),
                          g_empty_string);

  if (numeric_value > step_range.Maximum())
    return std::make_pair(RangeOverflowText(step_range.Maximum()),
                          g_empty_string);

  if (step_range.StepMismatch(numeric_value)) {
    DCHECK(step_range.HasStep());
    Decimal candidate1 = step_range.ClampValue(numeric_value);
    String localized_candidate1 = LocalizeValue(Serialize(candidate1));
    Decimal candidate2 = candidate1 < numeric_value
                             ? candidate1 + step_range.Step()
                             : candidate1 - step_range.Step();
    if (!candidate2.IsFinite() || candidate2 < step_range.Minimum() ||
        candidate2 > step_range.Maximum()) {
      return std::make_pair(
          GetLocale().QueryString(
              WebLocalizedString::kValidationStepMismatchCloseToLimit,
              localized_candidate1),
          g_empty_string);
    }
    String localized_candidate2 = LocalizeValue(Serialize(candidate2));
    if (candidate1 < candidate2) {
      return std::make_pair(
          GetLocale().QueryString(WebLocalizedString::kValidationStepMismatch,
                                  localized_candidate1, localized_candidate2),
          g_empty_string);
    }
    return std::make_pair(
        GetLocale().QueryString(WebLocalizedString::kValidationStepMismatch,
                                localized_candidate2, localized_candidate1),
        g_empty_string);
  }

  return std::make_pair(g_empty_string, g_empty_string);
}

Decimal InputType::ParseToNumber(const String&,
                                 const Decimal& default_value) const {
  NOTREACHED();
  return default_value;
}

Decimal InputType::ParseToNumberOrNaN(const String& string) const {
  return ParseToNumber(string, Decimal::Nan());
}

String InputType::Serialize(const Decimal&) const {
  NOTREACHED();
  return String();
}

ChromeClient* InputType::GetChromeClient() const {
  if (Page* page = GetElement().GetDocument().GetPage())
    return &page->GetChromeClient();
  return nullptr;
}

Locale& InputType::GetLocale() const {
  return GetElement().GetLocale();
}

bool InputType::CanSetStringValue() const {
  return true;
}

bool InputType::IsKeyboardFocusable() const {
  return GetElement().IsFocusable();
}

bool InputType::MayTriggerVirtualKeyboard() const {
  return false;
}

void InputType::CountUsage() {}

bool InputType::ShouldRespectAlignAttribute() {
  return false;
}

void InputType::SanitizeValueInResponseToMinOrMaxAttributeChange() {}

bool InputType::CanBeSuccessfulSubmitButton() {
  return false;
}

bool InputType::MatchesDefaultPseudoClass() {
  return false;
}

bool InputType::LayoutObjectIsNeeded() {
  return true;
}

FileList* InputType::Files() {
  return nullptr;
}

bool InputType::SetFiles(FileList*) {
  return false;
}

void InputType::SetFilesAndDispatchEvents(FileList*) {}

void InputType::SetFilesFromPaths(const Vector<String>& paths) {}

String InputType::ValueInFilenameValueMode() const {
  NOTREACHED();
  return String();
}

String InputType::DefaultLabel() const {
  return String();
}

bool InputType::CanSetSuggestedValue() {
  return false;
}

bool InputType::ShouldSendChangeEventAfterCheckedChanged() {
  return true;
}

void InputType::DispatchSearchEvent() {}

void InputType::SetValue(const String& sanitized_value,
                         bool value_changed,
                         TextFieldEventBehavior event_behavior,
                         TextControlSetValueSelection) {
  // This setValue() implementation is used only for ValueMode::kValue except
  // TextFieldInputType. That is to say, type=color, type=range, and temporal
  // input types.
  DCHECK_EQ(GetValueMode(), ValueMode::kValue);
  if (event_behavior == TextFieldEventBehavior::kDispatchNoEvent)
    GetElement().SetNonAttributeValue(sanitized_value);
  else
    GetElement().SetNonAttributeValueByUserEdit(sanitized_value);
  if (!value_changed)
    return;
  switch (event_behavior) {
    case TextFieldEventBehavior::kDispatchChangeEvent:
      GetElement().DispatchFormControlChangeEvent();
      break;
    case TextFieldEventBehavior::kDispatchInputAndChangeEvent:
      GetElement().DispatchInputEvent();
      GetElement().DispatchFormControlChangeEvent();
      break;
    case TextFieldEventBehavior::kDispatchNoEvent:
      break;
  }
}

bool InputType::CanSetValue(const String&) {
  return true;
}

String InputType::LocalizeValue(const String& proposed_value) const {
  return proposed_value;
}

String InputType::VisibleValue() const {
  return GetElement().value();
}

String InputType::SanitizeValue(const String& proposed_value) const {
  return proposed_value;
}

String InputType::SanitizeUserInputValue(const String& proposed_value) const {
  return SanitizeValue(proposed_value);
}

void InputType::WarnIfValueIsInvalidAndElementIsVisible(
    const String& value) const {
  // Don't warn if the value is set in Modernizr.
  const ComputedStyle* style = GetElement().GetComputedStyle();
  if (style && style->Visibility() != EVisibility::kHidden)
    WarnIfValueIsInvalid(value);
}

void InputType::WarnIfValueIsInvalid(const String&) const {}

bool InputType::ReceiveDroppedFiles(const DragData*) {
  NOTREACHED();
  return false;
}

String InputType::DroppedFileSystemId() {
  NOTREACHED();
  return String();
}

bool InputType::ShouldRespectListAttribute() {
  return false;
}

bool InputType::IsTextButton() const {
  return false;
}

bool InputType::IsInteractiveContent() const {
  return true;
}

bool InputType::IsEnumeratable() {
  return true;
}

bool InputType::IsCheckable() {
  return false;
}

bool InputType::IsSteppable() const {
  return false;
}

bool InputType::ShouldRespectHeightAndWidthAttributes() {
  return false;
}

int InputType::MaxLength() const {
  return -1;
}

int InputType::MinLength() const {
  return 0;
}

bool InputType::SupportsPlaceholder() const {
  return false;
}

bool InputType::SupportsReadOnly() const {
  return false;
}

String InputType::DefaultToolTip(const InputTypeView& input_type_view) const {
  if (GetElement().Form() && GetElement().Form()->NoValidate())
    return String();
  return ValidationMessage(input_type_view).first;
}

Decimal InputType::FindClosestTickMarkValue(const Decimal&) {
  NOTREACHED();
  return Decimal::Nan();
}

bool InputType::HasLegalLinkAttribute(const QualifiedName&) const {
  return false;
}

const QualifiedName& InputType::SubResourceAttributeName() const {
  return QualifiedName::Null();
}

void InputType::CopyNonAttributeProperties(const HTMLInputElement&) {}

void InputType::OnAttachWithLayoutObject() {}

bool InputType::ShouldAppearIndeterminate() const {
  return false;
}

bool InputType::SupportsInputModeAttribute() const {
  return false;
}

bool InputType::SupportsSelectionAPI() const {
  return false;
}

unsigned InputType::Height() const {
  return 0;
}

unsigned InputType::Width() const {
  return 0;
}

ColorChooserClient* InputType::GetColorChooserClient() {
  return nullptr;
}

void InputType::ApplyStep(const Decimal& current,
                          double count,
                          AnyStepHandling any_step_handling,
                          TextFieldEventBehavior event_behavior,
                          ExceptionState& exception_state) {
  // https://html.spec.whatwg.org/C/#dom-input-stepup

  StepRange step_range(CreateStepRange(any_step_handling));
  // 2. If the element has no allowed value step, then throw an
  // InvalidStateError exception, and abort these steps.
  if (!step_range.HasStep()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "This form element does not have an allowed value step.");
    return;
  }

  // 3. If the element has a minimum and a maximum and the minimum is greater
  // than the maximum, then abort these steps.
  if (step_range.Minimum() > step_range.Maximum())
    return;

  // 4. If the element has a minimum and a maximum and there is no value
  // greater than or equal to the element's minimum and less than or equal to
  // the element's maximum that, when subtracted from the step base, is an
  // integral multiple of the allowed value step, then abort these steps.
  Decimal aligned_maximum = step_range.StepSnappedMaximum();
  if (!aligned_maximum.IsFinite())
    return;

  Decimal base = step_range.StepBase();
  Decimal step = step_range.Step();
  EventQueueScope scope;
  Decimal new_value = current;
  const AtomicString& step_string = GetElement().FastGetAttribute(kStepAttr);
  if (!DeprecatedEqualIgnoringCase(step_string, "any") &&
      step_range.StepMismatch(current)) {
    // Snap-to-step / clamping steps
    // If the current value is not matched to step value:
    // - The value should be the larger matched value nearest to 0 if count > 0
    //   e.g. <input type=number value=3 min=-100 step=3> -> 5
    // - The value should be the smaller matched value nearest to 0 if count < 0
    //   e.g. <input type=number value=3 min=-100 step=3> -> 2
    //

    DCHECK(!step.IsZero());
    if (count < 0) {
      new_value = base + ((new_value - base) / step).Floor() * step;
      ++count;
    } else if (count > 0) {
      new_value = base + ((new_value - base) / step).Ceil() * step;
      --count;
    }
  }
  new_value = new_value + step_range.Step() * Decimal::FromDouble(count);

  if (!DeprecatedEqualIgnoringCase(step_string, "any"))
    new_value = step_range.AlignValueForStep(current, new_value);

  // 7. If the element has a minimum, and value is less than that minimum,
  // then set value to the smallest value that, when subtracted from the step
  // base, is an integral multiple of the allowed value step, and that is more
  // than or equal to minimum.
  if (new_value < step_range.Minimum()) {
    const Decimal aligned_minimum =
        base + ((step_range.Minimum() - base) / step).Ceil() * step;
    DCHECK_GE(aligned_minimum, step_range.Minimum());
    new_value = aligned_minimum;
  }

  // 8. If the element has a maximum, and value is greater than that maximum,
  // then set value to the largest value that, when subtracted from the step
  // base, is an integral multiple of the allowed value step, and that is less
  // than or equal to maximum.
  if (new_value > step_range.Maximum())
    new_value = aligned_maximum;

  // 9. Let value as string be the result of running the algorithm to convert
  // a number to a string, as defined for the input element's type attribute's
  // current state, on value.
  // 10. Set the value of the element to value as string.
  SetValueAsDecimal(new_value, event_behavior, exception_state);

  if (AXObjectCache* cache = GetElement().GetDocument().ExistingAXObjectCache())
    cache->HandleValueChanged(&GetElement());
}

bool InputType::GetAllowedValueStep(Decimal* step) const {
  StepRange step_range(CreateStepRange(kRejectAny));
  *step = step_range.Step();
  return step_range.HasStep();
}

StepRange InputType::CreateStepRange(AnyStepHandling) const {
  NOTREACHED();
  return StepRange();
}

void InputType::StepUp(double n, ExceptionState& exception_state) {
  if (!IsSteppable()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "This form element is not steppable.");
    return;
  }
  const Decimal current = ParseToNumber(GetElement().value(), 0);
  ApplyStep(current, n, kRejectAny, TextFieldEventBehavior::kDispatchNoEvent,
            exception_state);
}

void InputType::StepUpFromLayoutObject(int n) {
  // The only difference from stepUp()/stepDown() is the extra treatment
  // of the current value before applying the step:
  //
  // If the current value is not a number, including empty, the current value is
  // assumed as 0.
  //   * If 0 is in-range, and matches to step value
  //     - The value should be the +step if n > 0
  //     - The value should be the -step if n < 0
  //     If -step or +step is out of range, new value should be 0.
  //   * If 0 is smaller than the minimum value
  //     - The value should be the minimum value for any n
  //   * If 0 is larger than the maximum value
  //     - The value should be the maximum value for any n
  //   * If 0 is in-range, but not matched to step value
  //     - The value should be the larger matched value nearest to 0 if n > 0
  //       e.g. <input type=number min=-100 step=3> -> 2
  //     - The value should be the smaler matched value nearest to 0 if n < 0
  //       e.g. <input type=number min=-100 step=3> -> -1
  //   As for date/datetime-local/month/time/week types, the current value is
  //   assumed as "the current local date/time".
  //   As for datetime type, the current value is assumed as "the current
  //   date/time in UTC".
  // If the current value is smaller than the minimum value:
  //  - The value should be the minimum value if n > 0
  //  - Nothing should happen if n < 0
  // If the current value is larger than the maximum value:
  //  - The value should be the maximum value if n < 0
  //  - Nothing should happen if n > 0
  //
  // n is assumed as -n if step < 0.

  DCHECK(IsSteppable());
  if (!IsSteppable())
    return;
  DCHECK(n);
  if (!n)
    return;

  StepRange step_range(CreateStepRange(kAnyIsDefaultStep));

  // FIXME: Not any changes after stepping, even if it is an invalid value, may
  // be better.
  // (e.g. Stepping-up for <input type="number" value="foo" step="any" /> =>
  // "foo")
  if (!step_range.HasStep())
    return;

  EventQueueScope scope;
  const Decimal step = step_range.Step();

  int sign;
  if (step > 0)
    sign = n;
  else if (step < 0)
    sign = -n;
  else
    sign = 0;

  Decimal current = ParseToNumberOrNaN(GetElement().value());
  if (!current.IsFinite()) {
    current = DefaultValueForStepUp();
    const Decimal next_diff = step * n;
    if (current < step_range.Minimum() - next_diff)
      current = step_range.Minimum() - next_diff;
    if (current > step_range.Maximum() - next_diff)
      current = step_range.Maximum() - next_diff;
    SetValueAsDecimal(current, TextFieldEventBehavior::kDispatchNoEvent,
                      IGNORE_EXCEPTION_FOR_TESTING);
  }
  if ((sign > 0 && current < step_range.Minimum()) ||
      (sign < 0 && current > step_range.Maximum())) {
    SetValueAsDecimal(sign > 0 ? step_range.Minimum() : step_range.Maximum(),
                      TextFieldEventBehavior::kDispatchChangeEvent,
                      IGNORE_EXCEPTION_FOR_TESTING);
    return;
  }
  if ((sign > 0 && current >= step_range.Maximum()) ||
      (sign < 0 && current <= step_range.Minimum()))
    return;
  ApplyStep(current, n, kAnyIsDefaultStep,
            TextFieldEventBehavior::kDispatchChangeEvent,
            IGNORE_EXCEPTION_FOR_TESTING);
}

void InputType::CountUsageIfVisible(WebFeature feature) const {
  if (const ComputedStyle* style = GetElement().GetComputedStyle()) {
    if (style->Visibility() != EVisibility::kHidden)
      UseCounter::Count(GetElement().GetDocument(), feature);
  }
}

Decimal InputType::FindStepBase(const Decimal& default_value) const {
  Decimal step_base =
      ParseToNumber(GetElement().FastGetAttribute(kMinAttr), Decimal::Nan());
  if (!step_base.IsFinite()) {
    step_base =
        ParseToNumber(GetElement().FastGetAttribute(kValueAttr), default_value);
  }
  return step_base;
}

StepRange InputType::CreateStepRange(
    AnyStepHandling any_step_handling,
    const Decimal& step_base_default,
    const Decimal& minimum_default,
    const Decimal& maximum_default,
    const StepRange::StepDescription& step_description) const {
  bool has_range_limitations = false;
  const Decimal step_base = FindStepBase(step_base_default);
  Decimal minimum = ParseToNumberOrNaN(GetElement().FastGetAttribute(kMinAttr));
  if (minimum.IsFinite())
    has_range_limitations = true;
  else
    minimum = minimum_default;
  Decimal maximum = ParseToNumberOrNaN(GetElement().FastGetAttribute(kMaxAttr));
  if (maximum.IsFinite())
    has_range_limitations = true;
  else
    maximum = maximum_default;
  const Decimal step =
      StepRange::ParseStep(any_step_handling, step_description,
                           GetElement().FastGetAttribute(kStepAttr));
  return StepRange(step_base, minimum, maximum, has_range_limitations, step,
                   step_description);
}

void InputType::AddWarningToConsole(const char* message_format,
                                    const String& value) const {
  GetElement().GetDocument().AddConsoleMessage(ConsoleMessage::Create(
      mojom::ConsoleMessageSource::kRendering,
      mojom::ConsoleMessageLevel::kWarning,
      String::Format(message_format,
                     JSONValue::QuoteString(value).Utf8().data())));
}

}  // namespace blink
