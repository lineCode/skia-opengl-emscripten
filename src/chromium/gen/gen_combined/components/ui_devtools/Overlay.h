// This file is generated by TypeBuilder_h.template.

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ui_devtools_protocol_Overlay_h
#define ui_devtools_protocol_Overlay_h

#include "components/ui_devtools/devtools_export.h"
#include "components/ui_devtools/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "components/ui_devtools/DOM.h"

namespace ui_devtools {
namespace protocol {
namespace Overlay {

// ------------- Forward and enum declarations.
class HighlightConfig;
using InspectMode = String;
class InspectNodeRequestedNotification;
class NodeHighlightRequestedNotification;

namespace InspectModeEnum {
UI_DEVTOOLS_EXPORT extern const char SearchForNode[];
UI_DEVTOOLS_EXPORT extern const char SearchForUAShadowDOM[];
UI_DEVTOOLS_EXPORT extern const char None[];
} // namespace InspectModeEnum

// ------------- Type and builder declarations.

class UI_DEVTOOLS_EXPORT HighlightConfig : public Serializable{
    PROTOCOL_DISALLOW_COPY(HighlightConfig);
public:
    static std::unique_ptr<HighlightConfig> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~HighlightConfig() override { }

    bool hasShowInfo() { return m_showInfo.isJust(); }
    bool getShowInfo(bool defaultValue) { return m_showInfo.isJust() ? m_showInfo.fromJust() : defaultValue; }
    void setShowInfo(bool value) { m_showInfo = value; }

    bool hasShowRulers() { return m_showRulers.isJust(); }
    bool getShowRulers(bool defaultValue) { return m_showRulers.isJust() ? m_showRulers.fromJust() : defaultValue; }
    void setShowRulers(bool value) { m_showRulers = value; }

    bool hasShowExtensionLines() { return m_showExtensionLines.isJust(); }
    bool getShowExtensionLines(bool defaultValue) { return m_showExtensionLines.isJust() ? m_showExtensionLines.fromJust() : defaultValue; }
    void setShowExtensionLines(bool value) { m_showExtensionLines = value; }

    bool hasDisplayAsMaterial() { return m_displayAsMaterial.isJust(); }
    bool getDisplayAsMaterial(bool defaultValue) { return m_displayAsMaterial.isJust() ? m_displayAsMaterial.fromJust() : defaultValue; }
    void setDisplayAsMaterial(bool value) { m_displayAsMaterial = value; }

    bool hasContentColor() { return m_contentColor.isJust(); }
    protocol::DOM::RGBA* getContentColor(protocol::DOM::RGBA* defaultValue) { return m_contentColor.isJust() ? m_contentColor.fromJust() : defaultValue; }
    void setContentColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_contentColor = std::move(value); }

    bool hasPaddingColor() { return m_paddingColor.isJust(); }
    protocol::DOM::RGBA* getPaddingColor(protocol::DOM::RGBA* defaultValue) { return m_paddingColor.isJust() ? m_paddingColor.fromJust() : defaultValue; }
    void setPaddingColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_paddingColor = std::move(value); }

    bool hasBorderColor() { return m_borderColor.isJust(); }
    protocol::DOM::RGBA* getBorderColor(protocol::DOM::RGBA* defaultValue) { return m_borderColor.isJust() ? m_borderColor.fromJust() : defaultValue; }
    void setBorderColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_borderColor = std::move(value); }

    bool hasMarginColor() { return m_marginColor.isJust(); }
    protocol::DOM::RGBA* getMarginColor(protocol::DOM::RGBA* defaultValue) { return m_marginColor.isJust() ? m_marginColor.fromJust() : defaultValue; }
    void setMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_marginColor = std::move(value); }

    bool hasEventTargetColor() { return m_eventTargetColor.isJust(); }
    protocol::DOM::RGBA* getEventTargetColor(protocol::DOM::RGBA* defaultValue) { return m_eventTargetColor.isJust() ? m_eventTargetColor.fromJust() : defaultValue; }
    void setEventTargetColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_eventTargetColor = std::move(value); }

    bool hasShapeColor() { return m_shapeColor.isJust(); }
    protocol::DOM::RGBA* getShapeColor(protocol::DOM::RGBA* defaultValue) { return m_shapeColor.isJust() ? m_shapeColor.fromJust() : defaultValue; }
    void setShapeColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_shapeColor = std::move(value); }

    bool hasShapeMarginColor() { return m_shapeMarginColor.isJust(); }
    protocol::DOM::RGBA* getShapeMarginColor(protocol::DOM::RGBA* defaultValue) { return m_shapeMarginColor.isJust() ? m_shapeMarginColor.fromJust() : defaultValue; }
    void setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBA> value) { m_shapeMarginColor = std::move(value); }

    bool hasSelectorList() { return m_selectorList.isJust(); }
    String getSelectorList(const String& defaultValue) { return m_selectorList.isJust() ? m_selectorList.fromJust() : defaultValue; }
    void setSelectorList(const String& value) { m_selectorList = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serializeToJSON() override { return toValue()->serializeToJSON(); }
    std::vector<uint8_t> serializeToBinary() override { return toValue()->serializeToBinary(); }
    String toJSON() const { return toValue()->toJSONString(); }
    std::unique_ptr<HighlightConfig> clone() const;

    template<int STATE>
    class HighlightConfigBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            AllFieldsSet = (0)};


        HighlightConfigBuilder<STATE>& setShowInfo(bool value)
        {
            m_result->setShowInfo(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShowRulers(bool value)
        {
            m_result->setShowRulers(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShowExtensionLines(bool value)
        {
            m_result->setShowExtensionLines(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setDisplayAsMaterial(bool value)
        {
            m_result->setDisplayAsMaterial(value);
            return *this;
        }

        HighlightConfigBuilder<STATE>& setContentColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setContentColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setPaddingColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setPaddingColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setBorderColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setBorderColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setMarginColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setMarginColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setEventTargetColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setEventTargetColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShapeColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setShapeColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setShapeMarginColor(std::unique_ptr<protocol::DOM::RGBA> value)
        {
            m_result->setShapeMarginColor(std::move(value));
            return *this;
        }

        HighlightConfigBuilder<STATE>& setSelectorList(const String& value)
        {
            m_result->setSelectorList(value);
            return *this;
        }

        std::unique_ptr<HighlightConfig> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class HighlightConfig;
        HighlightConfigBuilder() : m_result(new HighlightConfig()) { }

        template<int STEP> HighlightConfigBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<HighlightConfigBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Overlay::HighlightConfig> m_result;
    };

    static HighlightConfigBuilder<0> create()
    {
        return HighlightConfigBuilder<0>();
    }

private:
    HighlightConfig()
    {
    }

    Maybe<bool> m_showInfo;
    Maybe<bool> m_showRulers;
    Maybe<bool> m_showExtensionLines;
    Maybe<bool> m_displayAsMaterial;
    Maybe<protocol::DOM::RGBA> m_contentColor;
    Maybe<protocol::DOM::RGBA> m_paddingColor;
    Maybe<protocol::DOM::RGBA> m_borderColor;
    Maybe<protocol::DOM::RGBA> m_marginColor;
    Maybe<protocol::DOM::RGBA> m_eventTargetColor;
    Maybe<protocol::DOM::RGBA> m_shapeColor;
    Maybe<protocol::DOM::RGBA> m_shapeMarginColor;
    Maybe<String> m_selectorList;
};


class UI_DEVTOOLS_EXPORT InspectNodeRequestedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(InspectNodeRequestedNotification);
public:
    static std::unique_ptr<InspectNodeRequestedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~InspectNodeRequestedNotification() override { }

    int getBackendNodeId() { return m_backendNodeId; }
    void setBackendNodeId(int value) { m_backendNodeId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serializeToJSON() override { return toValue()->serializeToJSON(); }
    std::vector<uint8_t> serializeToBinary() override { return toValue()->serializeToBinary(); }
    String toJSON() const { return toValue()->toJSONString(); }
    std::unique_ptr<InspectNodeRequestedNotification> clone() const;

    template<int STATE>
    class InspectNodeRequestedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            BackendNodeIdSet = 1 << 1,
            AllFieldsSet = (BackendNodeIdSet | 0)};


        InspectNodeRequestedNotificationBuilder<STATE | BackendNodeIdSet>& setBackendNodeId(int value)
        {
            static_assert(!(STATE & BackendNodeIdSet), "property backendNodeId should not be set yet");
            m_result->setBackendNodeId(value);
            return castState<BackendNodeIdSet>();
        }

        std::unique_ptr<InspectNodeRequestedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class InspectNodeRequestedNotification;
        InspectNodeRequestedNotificationBuilder() : m_result(new InspectNodeRequestedNotification()) { }

        template<int STEP> InspectNodeRequestedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<InspectNodeRequestedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Overlay::InspectNodeRequestedNotification> m_result;
    };

    static InspectNodeRequestedNotificationBuilder<0> create()
    {
        return InspectNodeRequestedNotificationBuilder<0>();
    }

private:
    InspectNodeRequestedNotification()
    {
          m_backendNodeId = 0;
    }

    int m_backendNodeId;
};


class UI_DEVTOOLS_EXPORT NodeHighlightRequestedNotification : public Serializable{
    PROTOCOL_DISALLOW_COPY(NodeHighlightRequestedNotification);
public:
    static std::unique_ptr<NodeHighlightRequestedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

    ~NodeHighlightRequestedNotification() override { }

    int getNodeId() { return m_nodeId; }
    void setNodeId(int value) { m_nodeId = value; }

    std::unique_ptr<protocol::DictionaryValue> toValue() const;
    String serializeToJSON() override { return toValue()->serializeToJSON(); }
    std::vector<uint8_t> serializeToBinary() override { return toValue()->serializeToBinary(); }
    String toJSON() const { return toValue()->toJSONString(); }
    std::unique_ptr<NodeHighlightRequestedNotification> clone() const;

    template<int STATE>
    class NodeHighlightRequestedNotificationBuilder {
    public:
        enum {
            NoFieldsSet = 0,
            NodeIdSet = 1 << 1,
            AllFieldsSet = (NodeIdSet | 0)};


        NodeHighlightRequestedNotificationBuilder<STATE | NodeIdSet>& setNodeId(int value)
        {
            static_assert(!(STATE & NodeIdSet), "property nodeId should not be set yet");
            m_result->setNodeId(value);
            return castState<NodeIdSet>();
        }

        std::unique_ptr<NodeHighlightRequestedNotification> build()
        {
            static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
            return std::move(m_result);
        }

    private:
        friend class NodeHighlightRequestedNotification;
        NodeHighlightRequestedNotificationBuilder() : m_result(new NodeHighlightRequestedNotification()) { }

        template<int STEP> NodeHighlightRequestedNotificationBuilder<STATE | STEP>& castState()
        {
            return *reinterpret_cast<NodeHighlightRequestedNotificationBuilder<STATE | STEP>*>(this);
        }

        std::unique_ptr<protocol::Overlay::NodeHighlightRequestedNotification> m_result;
    };

    static NodeHighlightRequestedNotificationBuilder<0> create()
    {
        return NodeHighlightRequestedNotificationBuilder<0>();
    }

private:
    NodeHighlightRequestedNotification()
    {
          m_nodeId = 0;
    }

    int m_nodeId;
};


// ------------- Backend interface.

class UI_DEVTOOLS_EXPORT Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse disable() = 0;
    virtual DispatchResponse enable() = 0;
    virtual DispatchResponse hideHighlight() = 0;
    virtual DispatchResponse highlightNode(std::unique_ptr<protocol::Overlay::HighlightConfig> in_highlightConfig, Maybe<int> in_nodeId) = 0;
    virtual DispatchResponse setInspectMode(const String& in_mode, Maybe<protocol::Overlay::HighlightConfig> in_highlightConfig) = 0;

};

// ------------- Frontend interface.

class UI_DEVTOOLS_EXPORT Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
    void inspectNodeRequested(int backendNodeId);
    void nodeHighlightRequested(int nodeId);

    void flush();
    void sendRawJSONNotification(String);
    void sendRawCBORNotification(std::vector<uint8_t>);
private:
    FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class UI_DEVTOOLS_EXPORT Dispatcher {
public:
    static void wire(UberDispatcher*, Backend*);

private:
    Dispatcher() { }
};

// ------------- Metainfo.

class UI_DEVTOOLS_EXPORT Metainfo {
public:
    using BackendClass = Backend;
    using FrontendClass = Frontend;
    using DispatcherClass = Dispatcher;
    static const char domainName[];
    static const char commandPrefix[];
    static const char version[];
};

} // namespace Overlay
} // namespace ui_devtools
} // namespace protocol

#endif // !defined(ui_devtools_protocol_Overlay_h)
