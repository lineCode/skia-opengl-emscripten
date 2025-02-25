// This file is generated by TypeBuilder_cpp.template.

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/devtools/protocol/input.h"

#include "content/browser/devtools/protocol/protocol.h"

namespace content {
namespace protocol {
namespace Input {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Input";
const char Metainfo::commandPrefix[] = "Input.";
const char Metainfo::version[] = "1.3";

std::unique_ptr<TouchPoint> TouchPoint::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<TouchPoint> result(new TouchPoint());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* xValue = object->get("x");
    errors->setName("x");
    result->m_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object->get("y");
    errors->setName("y");
    result->m_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* radiusXValue = object->get("radiusX");
    if (radiusXValue) {
        errors->setName("radiusX");
        result->m_radiusX = ValueConversions<double>::fromValue(radiusXValue, errors);
    }
    protocol::Value* radiusYValue = object->get("radiusY");
    if (radiusYValue) {
        errors->setName("radiusY");
        result->m_radiusY = ValueConversions<double>::fromValue(radiusYValue, errors);
    }
    protocol::Value* rotationAngleValue = object->get("rotationAngle");
    if (rotationAngleValue) {
        errors->setName("rotationAngle");
        result->m_rotationAngle = ValueConversions<double>::fromValue(rotationAngleValue, errors);
    }
    protocol::Value* forceValue = object->get("force");
    if (forceValue) {
        errors->setName("force");
        result->m_force = ValueConversions<double>::fromValue(forceValue, errors);
    }
    protocol::Value* idValue = object->get("id");
    if (idValue) {
        errors->setName("id");
        result->m_id = ValueConversions<double>::fromValue(idValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> TouchPoint::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("x", ValueConversions<double>::toValue(m_x));
    result->setValue("y", ValueConversions<double>::toValue(m_y));
    if (m_radiusX.isJust())
        result->setValue("radiusX", ValueConversions<double>::toValue(m_radiusX.fromJust()));
    if (m_radiusY.isJust())
        result->setValue("radiusY", ValueConversions<double>::toValue(m_radiusY.fromJust()));
    if (m_rotationAngle.isJust())
        result->setValue("rotationAngle", ValueConversions<double>::toValue(m_rotationAngle.fromJust()));
    if (m_force.isJust())
        result->setValue("force", ValueConversions<double>::toValue(m_force.fromJust()));
    if (m_id.isJust())
        result->setValue("id", ValueConversions<double>::toValue(m_id.fromJust()));
    return result;
}

std::unique_ptr<TouchPoint> TouchPoint::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

namespace GestureSourceTypeEnum {
const char Default[] = "default";
const char Touch[] = "touch";
const char Mouse[] = "mouse";
} // namespace GestureSourceTypeEnum

// ------------- Enum values from params.


namespace DispatchKeyEvent {
namespace TypeEnum {
const char* KeyDown = "keyDown";
const char* KeyUp = "keyUp";
const char* RawKeyDown = "rawKeyDown";
const char* Char = "char";
} // namespace TypeEnum
} // namespace DispatchKeyEvent

namespace DispatchMouseEvent {
namespace TypeEnum {
const char* MousePressed = "mousePressed";
const char* MouseReleased = "mouseReleased";
const char* MouseMoved = "mouseMoved";
const char* MouseWheel = "mouseWheel";
} // namespace TypeEnum
} // namespace DispatchMouseEvent

namespace DispatchMouseEvent {
namespace ButtonEnum {
const char* None = "none";
const char* Left = "left";
const char* Middle = "middle";
const char* Right = "right";
const char* Back = "back";
const char* Forward = "forward";
} // namespace ButtonEnum
} // namespace DispatchMouseEvent

namespace DispatchMouseEvent {
namespace PointerTypeEnum {
const char* Mouse = "mouse";
const char* Pen = "pen";
} // namespace PointerTypeEnum
} // namespace DispatchMouseEvent

namespace DispatchTouchEvent {
namespace TypeEnum {
const char* TouchStart = "touchStart";
const char* TouchEnd = "touchEnd";
const char* TouchMove = "touchMove";
const char* TouchCancel = "touchCancel";
} // namespace TypeEnum
} // namespace DispatchTouchEvent

namespace EmulateTouchFromMouseEvent {
namespace TypeEnum {
const char* MousePressed = "mousePressed";
const char* MouseReleased = "mouseReleased";
const char* MouseMoved = "mouseMoved";
const char* MouseWheel = "mouseWheel";
} // namespace TypeEnum
} // namespace EmulateTouchFromMouseEvent

namespace EmulateTouchFromMouseEvent {
namespace ButtonEnum {
const char* None = "none";
const char* Left = "left";
const char* Middle = "middle";
const char* Right = "right";
} // namespace ButtonEnum
} // namespace EmulateTouchFromMouseEvent

// ------------- Frontend notifications.

void Frontend::flush()
{
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawJSONNotification(String notification)
{
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::fromJSON(std::move(notification)));
}

void Frontend::sendRawCBORNotification(std::vector<uint8_t> notification)
{
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::fromBinary(std::move(notification)));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend)
        : DispatcherBase(frontendChannel)
        , m_backend(backend) {
        m_dispatchMap["Input.dispatchKeyEvent"] = &DispatcherImpl::dispatchKeyEvent;
        m_dispatchMap["Input.insertText"] = &DispatcherImpl::insertText;
        m_dispatchMap["Input.dispatchMouseEvent"] = &DispatcherImpl::dispatchMouseEvent;
        m_dispatchMap["Input.dispatchTouchEvent"] = &DispatcherImpl::dispatchTouchEvent;
        m_dispatchMap["Input.emulateTouchFromMouseEvent"] = &DispatcherImpl::emulateTouchFromMouseEvent;
        m_dispatchMap["Input.setIgnoreInputEvents"] = &DispatcherImpl::setIgnoreInputEvents;
        m_dispatchMap["Input.synthesizePinchGesture"] = &DispatcherImpl::synthesizePinchGesture;
        m_dispatchMap["Input.synthesizeScrollGesture"] = &DispatcherImpl::synthesizeScrollGesture;
        m_dispatchMap["Input.synthesizeTapGesture"] = &DispatcherImpl::synthesizeTapGesture;
    }
    ~DispatcherImpl() override { }
    bool canDispatch(const String& method) override;
    void dispatch(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
    std::unordered_map<String, String>& redirects() { return m_redirects; }

protected:
    using CallHandler = void (DispatcherImpl::*)(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = std::unordered_map<String, CallHandler>;
    DispatchMap m_dispatchMap;
    std::unordered_map<String, String> m_redirects;

    void dispatchKeyEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void insertText(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void dispatchMouseEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void dispatchTouchEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void emulateTouchFromMouseEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void setIgnoreInputEvents(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void synthesizePinchGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void synthesizeScrollGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void synthesizeTapGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
};

bool DispatcherImpl::canDispatch(const String& method) {
    return m_dispatchMap.find(method) != m_dispatchMap.end();
}

void DispatcherImpl::dispatch(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    std::unordered_map<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    DCHECK(it != m_dispatchMap.end());
    protocol::ErrorSupport errors;
    (this->*(it->second))(callId, method, message, std::move(messageObject), &errors);
}


class DispatchKeyEventCallbackImpl : public Backend::DispatchKeyEventCallback, public DispatcherBase::Callback {
public:
    DispatchKeyEventCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::dispatchKeyEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* typeValue = object ? object->get("type") : nullptr;
    errors->setName("type");
    String in_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* modifiersValue = object ? object->get("modifiers") : nullptr;
    Maybe<int> in_modifiers;
    if (modifiersValue) {
        errors->setName("modifiers");
        in_modifiers = ValueConversions<int>::fromValue(modifiersValue, errors);
    }
    protocol::Value* timestampValue = object ? object->get("timestamp") : nullptr;
    Maybe<double> in_timestamp;
    if (timestampValue) {
        errors->setName("timestamp");
        in_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    }
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    Maybe<String> in_text;
    if (textValue) {
        errors->setName("text");
        in_text = ValueConversions<String>::fromValue(textValue, errors);
    }
    protocol::Value* unmodifiedTextValue = object ? object->get("unmodifiedText") : nullptr;
    Maybe<String> in_unmodifiedText;
    if (unmodifiedTextValue) {
        errors->setName("unmodifiedText");
        in_unmodifiedText = ValueConversions<String>::fromValue(unmodifiedTextValue, errors);
    }
    protocol::Value* keyIdentifierValue = object ? object->get("keyIdentifier") : nullptr;
    Maybe<String> in_keyIdentifier;
    if (keyIdentifierValue) {
        errors->setName("keyIdentifier");
        in_keyIdentifier = ValueConversions<String>::fromValue(keyIdentifierValue, errors);
    }
    protocol::Value* codeValue = object ? object->get("code") : nullptr;
    Maybe<String> in_code;
    if (codeValue) {
        errors->setName("code");
        in_code = ValueConversions<String>::fromValue(codeValue, errors);
    }
    protocol::Value* keyValue = object ? object->get("key") : nullptr;
    Maybe<String> in_key;
    if (keyValue) {
        errors->setName("key");
        in_key = ValueConversions<String>::fromValue(keyValue, errors);
    }
    protocol::Value* windowsVirtualKeyCodeValue = object ? object->get("windowsVirtualKeyCode") : nullptr;
    Maybe<int> in_windowsVirtualKeyCode;
    if (windowsVirtualKeyCodeValue) {
        errors->setName("windowsVirtualKeyCode");
        in_windowsVirtualKeyCode = ValueConversions<int>::fromValue(windowsVirtualKeyCodeValue, errors);
    }
    protocol::Value* nativeVirtualKeyCodeValue = object ? object->get("nativeVirtualKeyCode") : nullptr;
    Maybe<int> in_nativeVirtualKeyCode;
    if (nativeVirtualKeyCodeValue) {
        errors->setName("nativeVirtualKeyCode");
        in_nativeVirtualKeyCode = ValueConversions<int>::fromValue(nativeVirtualKeyCodeValue, errors);
    }
    protocol::Value* autoRepeatValue = object ? object->get("autoRepeat") : nullptr;
    Maybe<bool> in_autoRepeat;
    if (autoRepeatValue) {
        errors->setName("autoRepeat");
        in_autoRepeat = ValueConversions<bool>::fromValue(autoRepeatValue, errors);
    }
    protocol::Value* isKeypadValue = object ? object->get("isKeypad") : nullptr;
    Maybe<bool> in_isKeypad;
    if (isKeypadValue) {
        errors->setName("isKeypad");
        in_isKeypad = ValueConversions<bool>::fromValue(isKeypadValue, errors);
    }
    protocol::Value* isSystemKeyValue = object ? object->get("isSystemKey") : nullptr;
    Maybe<bool> in_isSystemKey;
    if (isSystemKeyValue) {
        errors->setName("isSystemKey");
        in_isSystemKey = ValueConversions<bool>::fromValue(isSystemKeyValue, errors);
    }
    protocol::Value* locationValue = object ? object->get("location") : nullptr;
    Maybe<int> in_location;
    if (locationValue) {
        errors->setName("location");
        in_location = ValueConversions<int>::fromValue(locationValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<DispatchKeyEventCallbackImpl> callback(new DispatchKeyEventCallbackImpl(weakPtr(), callId, method, message));
    m_backend->DispatchKeyEvent(in_type, std::move(in_modifiers), std::move(in_timestamp), std::move(in_text), std::move(in_unmodifiedText), std::move(in_keyIdentifier), std::move(in_code), std::move(in_key), std::move(in_windowsVirtualKeyCode), std::move(in_nativeVirtualKeyCode), std::move(in_autoRepeat), std::move(in_isKeypad), std::move(in_isSystemKey), std::move(in_location), std::move(callback));
    return;
}

class InsertTextCallbackImpl : public Backend::InsertTextCallback, public DispatcherBase::Callback {
public:
    InsertTextCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::insertText(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* textValue = object ? object->get("text") : nullptr;
    errors->setName("text");
    String in_text = ValueConversions<String>::fromValue(textValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<InsertTextCallbackImpl> callback(new InsertTextCallbackImpl(weakPtr(), callId, method, message));
    m_backend->InsertText(in_text, std::move(callback));
    return;
}

class DispatchMouseEventCallbackImpl : public Backend::DispatchMouseEventCallback, public DispatcherBase::Callback {
public:
    DispatchMouseEventCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::dispatchMouseEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* typeValue = object ? object->get("type") : nullptr;
    errors->setName("type");
    String in_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    double in_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    double in_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* modifiersValue = object ? object->get("modifiers") : nullptr;
    Maybe<int> in_modifiers;
    if (modifiersValue) {
        errors->setName("modifiers");
        in_modifiers = ValueConversions<int>::fromValue(modifiersValue, errors);
    }
    protocol::Value* timestampValue = object ? object->get("timestamp") : nullptr;
    Maybe<double> in_timestamp;
    if (timestampValue) {
        errors->setName("timestamp");
        in_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    }
    protocol::Value* buttonValue = object ? object->get("button") : nullptr;
    Maybe<String> in_button;
    if (buttonValue) {
        errors->setName("button");
        in_button = ValueConversions<String>::fromValue(buttonValue, errors);
    }
    protocol::Value* buttonsValue = object ? object->get("buttons") : nullptr;
    Maybe<int> in_buttons;
    if (buttonsValue) {
        errors->setName("buttons");
        in_buttons = ValueConversions<int>::fromValue(buttonsValue, errors);
    }
    protocol::Value* clickCountValue = object ? object->get("clickCount") : nullptr;
    Maybe<int> in_clickCount;
    if (clickCountValue) {
        errors->setName("clickCount");
        in_clickCount = ValueConversions<int>::fromValue(clickCountValue, errors);
    }
    protocol::Value* deltaXValue = object ? object->get("deltaX") : nullptr;
    Maybe<double> in_deltaX;
    if (deltaXValue) {
        errors->setName("deltaX");
        in_deltaX = ValueConversions<double>::fromValue(deltaXValue, errors);
    }
    protocol::Value* deltaYValue = object ? object->get("deltaY") : nullptr;
    Maybe<double> in_deltaY;
    if (deltaYValue) {
        errors->setName("deltaY");
        in_deltaY = ValueConversions<double>::fromValue(deltaYValue, errors);
    }
    protocol::Value* pointerTypeValue = object ? object->get("pointerType") : nullptr;
    Maybe<String> in_pointerType;
    if (pointerTypeValue) {
        errors->setName("pointerType");
        in_pointerType = ValueConversions<String>::fromValue(pointerTypeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<DispatchMouseEventCallbackImpl> callback(new DispatchMouseEventCallbackImpl(weakPtr(), callId, method, message));
    m_backend->DispatchMouseEvent(in_type, in_x, in_y, std::move(in_modifiers), std::move(in_timestamp), std::move(in_button), std::move(in_buttons), std::move(in_clickCount), std::move(in_deltaX), std::move(in_deltaY), std::move(in_pointerType), std::move(callback));
    return;
}

class DispatchTouchEventCallbackImpl : public Backend::DispatchTouchEventCallback, public DispatcherBase::Callback {
public:
    DispatchTouchEventCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::dispatchTouchEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* typeValue = object ? object->get("type") : nullptr;
    errors->setName("type");
    String in_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* touchPointsValue = object ? object->get("touchPoints") : nullptr;
    errors->setName("touchPoints");
    std::unique_ptr<protocol::Array<protocol::Input::TouchPoint>> in_touchPoints = ValueConversions<protocol::Array<protocol::Input::TouchPoint>>::fromValue(touchPointsValue, errors);
    protocol::Value* modifiersValue = object ? object->get("modifiers") : nullptr;
    Maybe<int> in_modifiers;
    if (modifiersValue) {
        errors->setName("modifiers");
        in_modifiers = ValueConversions<int>::fromValue(modifiersValue, errors);
    }
    protocol::Value* timestampValue = object ? object->get("timestamp") : nullptr;
    Maybe<double> in_timestamp;
    if (timestampValue) {
        errors->setName("timestamp");
        in_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<DispatchTouchEventCallbackImpl> callback(new DispatchTouchEventCallbackImpl(weakPtr(), callId, method, message));
    m_backend->DispatchTouchEvent(in_type, std::move(in_touchPoints), std::move(in_modifiers), std::move(in_timestamp), std::move(callback));
    return;
}

void DispatcherImpl::emulateTouchFromMouseEvent(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* typeValue = object ? object->get("type") : nullptr;
    errors->setName("type");
    String in_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    int in_x = ValueConversions<int>::fromValue(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    int in_y = ValueConversions<int>::fromValue(yValue, errors);
    protocol::Value* buttonValue = object ? object->get("button") : nullptr;
    errors->setName("button");
    String in_button = ValueConversions<String>::fromValue(buttonValue, errors);
    protocol::Value* timestampValue = object ? object->get("timestamp") : nullptr;
    Maybe<double> in_timestamp;
    if (timestampValue) {
        errors->setName("timestamp");
        in_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    }
    protocol::Value* deltaXValue = object ? object->get("deltaX") : nullptr;
    Maybe<double> in_deltaX;
    if (deltaXValue) {
        errors->setName("deltaX");
        in_deltaX = ValueConversions<double>::fromValue(deltaXValue, errors);
    }
    protocol::Value* deltaYValue = object ? object->get("deltaY") : nullptr;
    Maybe<double> in_deltaY;
    if (deltaYValue) {
        errors->setName("deltaY");
        in_deltaY = ValueConversions<double>::fromValue(deltaYValue, errors);
    }
    protocol::Value* modifiersValue = object ? object->get("modifiers") : nullptr;
    Maybe<int> in_modifiers;
    if (modifiersValue) {
        errors->setName("modifiers");
        in_modifiers = ValueConversions<int>::fromValue(modifiersValue, errors);
    }
    protocol::Value* clickCountValue = object ? object->get("clickCount") : nullptr;
    Maybe<int> in_clickCount;
    if (clickCountValue) {
        errors->setName("clickCount");
        in_clickCount = ValueConversions<int>::fromValue(clickCountValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->EmulateTouchFromMouseEvent(in_type, in_x, in_y, in_button, std::move(in_timestamp), std::move(in_deltaX), std::move(in_deltaY), std::move(in_modifiers), std::move(in_clickCount));
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return;
}

void DispatcherImpl::setIgnoreInputEvents(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* ignoreValue = object ? object->get("ignore") : nullptr;
    errors->setName("ignore");
    bool in_ignore = ValueConversions<bool>::fromValue(ignoreValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->SetIgnoreInputEvents(in_ignore);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return;
}

class SynthesizePinchGestureCallbackImpl : public Backend::SynthesizePinchGestureCallback, public DispatcherBase::Callback {
public:
    SynthesizePinchGestureCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::synthesizePinchGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    double in_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    double in_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* scaleFactorValue = object ? object->get("scaleFactor") : nullptr;
    errors->setName("scaleFactor");
    double in_scaleFactor = ValueConversions<double>::fromValue(scaleFactorValue, errors);
    protocol::Value* relativeSpeedValue = object ? object->get("relativeSpeed") : nullptr;
    Maybe<int> in_relativeSpeed;
    if (relativeSpeedValue) {
        errors->setName("relativeSpeed");
        in_relativeSpeed = ValueConversions<int>::fromValue(relativeSpeedValue, errors);
    }
    protocol::Value* gestureSourceTypeValue = object ? object->get("gestureSourceType") : nullptr;
    Maybe<String> in_gestureSourceType;
    if (gestureSourceTypeValue) {
        errors->setName("gestureSourceType");
        in_gestureSourceType = ValueConversions<String>::fromValue(gestureSourceTypeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<SynthesizePinchGestureCallbackImpl> callback(new SynthesizePinchGestureCallbackImpl(weakPtr(), callId, method, message));
    m_backend->SynthesizePinchGesture(in_x, in_y, in_scaleFactor, std::move(in_relativeSpeed), std::move(in_gestureSourceType), std::move(callback));
    return;
}

class SynthesizeScrollGestureCallbackImpl : public Backend::SynthesizeScrollGestureCallback, public DispatcherBase::Callback {
public:
    SynthesizeScrollGestureCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::synthesizeScrollGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    double in_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    double in_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* xDistanceValue = object ? object->get("xDistance") : nullptr;
    Maybe<double> in_xDistance;
    if (xDistanceValue) {
        errors->setName("xDistance");
        in_xDistance = ValueConversions<double>::fromValue(xDistanceValue, errors);
    }
    protocol::Value* yDistanceValue = object ? object->get("yDistance") : nullptr;
    Maybe<double> in_yDistance;
    if (yDistanceValue) {
        errors->setName("yDistance");
        in_yDistance = ValueConversions<double>::fromValue(yDistanceValue, errors);
    }
    protocol::Value* xOverscrollValue = object ? object->get("xOverscroll") : nullptr;
    Maybe<double> in_xOverscroll;
    if (xOverscrollValue) {
        errors->setName("xOverscroll");
        in_xOverscroll = ValueConversions<double>::fromValue(xOverscrollValue, errors);
    }
    protocol::Value* yOverscrollValue = object ? object->get("yOverscroll") : nullptr;
    Maybe<double> in_yOverscroll;
    if (yOverscrollValue) {
        errors->setName("yOverscroll");
        in_yOverscroll = ValueConversions<double>::fromValue(yOverscrollValue, errors);
    }
    protocol::Value* preventFlingValue = object ? object->get("preventFling") : nullptr;
    Maybe<bool> in_preventFling;
    if (preventFlingValue) {
        errors->setName("preventFling");
        in_preventFling = ValueConversions<bool>::fromValue(preventFlingValue, errors);
    }
    protocol::Value* speedValue = object ? object->get("speed") : nullptr;
    Maybe<int> in_speed;
    if (speedValue) {
        errors->setName("speed");
        in_speed = ValueConversions<int>::fromValue(speedValue, errors);
    }
    protocol::Value* gestureSourceTypeValue = object ? object->get("gestureSourceType") : nullptr;
    Maybe<String> in_gestureSourceType;
    if (gestureSourceTypeValue) {
        errors->setName("gestureSourceType");
        in_gestureSourceType = ValueConversions<String>::fromValue(gestureSourceTypeValue, errors);
    }
    protocol::Value* repeatCountValue = object ? object->get("repeatCount") : nullptr;
    Maybe<int> in_repeatCount;
    if (repeatCountValue) {
        errors->setName("repeatCount");
        in_repeatCount = ValueConversions<int>::fromValue(repeatCountValue, errors);
    }
    protocol::Value* repeatDelayMsValue = object ? object->get("repeatDelayMs") : nullptr;
    Maybe<int> in_repeatDelayMs;
    if (repeatDelayMsValue) {
        errors->setName("repeatDelayMs");
        in_repeatDelayMs = ValueConversions<int>::fromValue(repeatDelayMsValue, errors);
    }
    protocol::Value* interactionMarkerNameValue = object ? object->get("interactionMarkerName") : nullptr;
    Maybe<String> in_interactionMarkerName;
    if (interactionMarkerNameValue) {
        errors->setName("interactionMarkerName");
        in_interactionMarkerName = ValueConversions<String>::fromValue(interactionMarkerNameValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<SynthesizeScrollGestureCallbackImpl> callback(new SynthesizeScrollGestureCallbackImpl(weakPtr(), callId, method, message));
    m_backend->SynthesizeScrollGesture(in_x, in_y, std::move(in_xDistance), std::move(in_yDistance), std::move(in_xOverscroll), std::move(in_yOverscroll), std::move(in_preventFling), std::move(in_speed), std::move(in_gestureSourceType), std::move(in_repeatCount), std::move(in_repeatDelayMs), std::move(in_interactionMarkerName), std::move(callback));
    return;
}

class SynthesizeTapGestureCallbackImpl : public Backend::SynthesizeTapGestureCallback, public DispatcherBase::Callback {
public:
    SynthesizeTapGestureCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, const String& method, const ProtocolMessage& message)
        : DispatcherBase::Callback(std::move(backendImpl), callId, method, message) { }

    void sendSuccess() override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

void DispatcherImpl::synthesizeTapGesture(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* xValue = object ? object->get("x") : nullptr;
    errors->setName("x");
    double in_x = ValueConversions<double>::fromValue(xValue, errors);
    protocol::Value* yValue = object ? object->get("y") : nullptr;
    errors->setName("y");
    double in_y = ValueConversions<double>::fromValue(yValue, errors);
    protocol::Value* durationValue = object ? object->get("duration") : nullptr;
    Maybe<int> in_duration;
    if (durationValue) {
        errors->setName("duration");
        in_duration = ValueConversions<int>::fromValue(durationValue, errors);
    }
    protocol::Value* tapCountValue = object ? object->get("tapCount") : nullptr;
    Maybe<int> in_tapCount;
    if (tapCountValue) {
        errors->setName("tapCount");
        in_tapCount = ValueConversions<int>::fromValue(tapCountValue, errors);
    }
    protocol::Value* gestureSourceTypeValue = object ? object->get("gestureSourceType") : nullptr;
    Maybe<String> in_gestureSourceType;
    if (gestureSourceTypeValue) {
        errors->setName("gestureSourceType");
        in_gestureSourceType = ValueConversions<String>::fromValue(gestureSourceTypeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<SynthesizeTapGestureCallbackImpl> callback(new SynthesizeTapGestureCallbackImpl(weakPtr(), callId, method, message));
    m_backend->SynthesizeTapGesture(in_x, in_y, std::move(in_duration), std::move(in_tapCount), std::move(in_gestureSourceType), std::move(callback));
    return;
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Input", std::move(dispatcher));
}

} // Input
} // namespace content
} // namespace protocol
