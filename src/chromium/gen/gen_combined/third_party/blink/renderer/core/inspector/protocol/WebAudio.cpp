// This file is generated by TypeBuilder_cpp.template.

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/inspector/protocol/WebAudio.h"

#include "third_party/blink/renderer/core/inspector/protocol/Protocol.h"

namespace blink {
namespace protocol {
namespace WebAudio {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "WebAudio";
const char Metainfo::commandPrefix[] = "WebAudio.";
const char Metainfo::version[] = "1.3";

namespace ContextTypeEnum {
const char Realtime[] = "realtime";
const char Offline[] = "offline";
} // namespace ContextTypeEnum

namespace ContextStateEnum {
const char Suspended[] = "suspended";
const char Running[] = "running";
const char Closed[] = "closed";
} // namespace ContextStateEnum

std::unique_ptr<ContextRealtimeData> ContextRealtimeData::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ContextRealtimeData> result(new ContextRealtimeData());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* currentTimeValue = object->get("currentTime");
    if (currentTimeValue) {
        errors->setName("currentTime");
        result->m_currentTime = ValueConversions<double>::fromValue(currentTimeValue, errors);
    }
    protocol::Value* renderCapacityValue = object->get("renderCapacity");
    if (renderCapacityValue) {
        errors->setName("renderCapacity");
        result->m_renderCapacity = ValueConversions<double>::fromValue(renderCapacityValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ContextRealtimeData::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_currentTime.isJust())
        result->setValue("currentTime", ValueConversions<double>::toValue(m_currentTime.fromJust()));
    if (m_renderCapacity.isJust())
        result->setValue("renderCapacity", ValueConversions<double>::toValue(m_renderCapacity.fromJust()));
    return result;
}

std::unique_ptr<ContextRealtimeData> ContextRealtimeData::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<BaseAudioContext> BaseAudioContext::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<BaseAudioContext> result(new BaseAudioContext());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* contextIdValue = object->get("contextId");
    errors->setName("contextId");
    result->m_contextId = ValueConversions<String>::fromValue(contextIdValue, errors);
    protocol::Value* contextTypeValue = object->get("contextType");
    errors->setName("contextType");
    result->m_contextType = ValueConversions<String>::fromValue(contextTypeValue, errors);
    protocol::Value* contextStateValue = object->get("contextState");
    errors->setName("contextState");
    result->m_contextState = ValueConversions<String>::fromValue(contextStateValue, errors);
    protocol::Value* realtimeDataValue = object->get("realtimeData");
    if (realtimeDataValue) {
        errors->setName("realtimeData");
        result->m_realtimeData = ValueConversions<protocol::WebAudio::ContextRealtimeData>::fromValue(realtimeDataValue, errors);
    }
    protocol::Value* callbackBufferSizeValue = object->get("callbackBufferSize");
    errors->setName("callbackBufferSize");
    result->m_callbackBufferSize = ValueConversions<double>::fromValue(callbackBufferSizeValue, errors);
    protocol::Value* maxOutputChannelCountValue = object->get("maxOutputChannelCount");
    errors->setName("maxOutputChannelCount");
    result->m_maxOutputChannelCount = ValueConversions<double>::fromValue(maxOutputChannelCountValue, errors);
    protocol::Value* sampleRateValue = object->get("sampleRate");
    errors->setName("sampleRate");
    result->m_sampleRate = ValueConversions<double>::fromValue(sampleRateValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> BaseAudioContext::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("contextId", ValueConversions<String>::toValue(m_contextId));
    result->setValue("contextType", ValueConversions<String>::toValue(m_contextType));
    result->setValue("contextState", ValueConversions<String>::toValue(m_contextState));
    if (m_realtimeData.isJust())
        result->setValue("realtimeData", ValueConversions<protocol::WebAudio::ContextRealtimeData>::toValue(m_realtimeData.fromJust()));
    result->setValue("callbackBufferSize", ValueConversions<double>::toValue(m_callbackBufferSize));
    result->setValue("maxOutputChannelCount", ValueConversions<double>::toValue(m_maxOutputChannelCount));
    result->setValue("sampleRate", ValueConversions<double>::toValue(m_sampleRate));
    return result;
}

std::unique_ptr<BaseAudioContext> BaseAudioContext::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ContextCreatedNotification> ContextCreatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ContextCreatedNotification> result(new ContextCreatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* contextValue = object->get("context");
    errors->setName("context");
    result->m_context = ValueConversions<protocol::WebAudio::BaseAudioContext>::fromValue(contextValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ContextCreatedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("context", ValueConversions<protocol::WebAudio::BaseAudioContext>::toValue(m_context.get()));
    return result;
}

std::unique_ptr<ContextCreatedNotification> ContextCreatedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ContextDestroyedNotification> ContextDestroyedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ContextDestroyedNotification> result(new ContextDestroyedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* contextIdValue = object->get("contextId");
    errors->setName("contextId");
    result->m_contextId = ValueConversions<String>::fromValue(contextIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ContextDestroyedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("contextId", ValueConversions<String>::toValue(m_contextId));
    return result;
}

std::unique_ptr<ContextDestroyedNotification> ContextDestroyedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ContextChangedNotification> ContextChangedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ContextChangedNotification> result(new ContextChangedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* contextValue = object->get("context");
    errors->setName("context");
    result->m_context = ValueConversions<protocol::WebAudio::BaseAudioContext>::fromValue(contextValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ContextChangedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("context", ValueConversions<protocol::WebAudio::BaseAudioContext>::toValue(m_context.get()));
    return result;
}

std::unique_ptr<ContextChangedNotification> ContextChangedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::contextCreated(std::unique_ptr<protocol::WebAudio::BaseAudioContext> context)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ContextCreatedNotification> messageData = ContextCreatedNotification::create()
        .setContext(std::move(context))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("WebAudio.contextCreated", std::move(messageData)));
}

void Frontend::contextDestroyed(const String& contextId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ContextDestroyedNotification> messageData = ContextDestroyedNotification::create()
        .setContextId(contextId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("WebAudio.contextDestroyed", std::move(messageData)));
}

void Frontend::contextChanged(std::unique_ptr<protocol::WebAudio::BaseAudioContext> context)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ContextChangedNotification> messageData = ContextChangedNotification::create()
        .setContext(std::move(context))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("WebAudio.contextChanged", std::move(messageData)));
}

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
        m_dispatchMap["WebAudio.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["WebAudio.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["WebAudio.getRealtimeData"] = &DispatcherImpl::getRealtimeData;
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

    void enable(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void disable(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    void getRealtimeData(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

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


void DispatcherImpl::enable(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return;
}

void DispatcherImpl::disable(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return;
}

void DispatcherImpl::getRealtimeData(int callId, const String& method, const ProtocolMessage& message, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* contextIdValue = object ? object->get("contextId") : nullptr;
    errors->setName("contextId");
    String in_contextId = ValueConversions<String>::fromValue(contextIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::WebAudio::ContextRealtimeData> out_realtimeData;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getRealtimeData(in_contextId, &out_realtimeData);
    if (response.status() == DispatchResponse::kFallThrough) {
        channel()->fallThrough(callId, method, message);
        return;
    }
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("realtimeData", ValueConversions<protocol::WebAudio::ContextRealtimeData>::toValue(out_realtimeData.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return;
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("WebAudio", std::move(dispatcher));
}

} // WebAudio
} // namespace blink
} // namespace protocol
