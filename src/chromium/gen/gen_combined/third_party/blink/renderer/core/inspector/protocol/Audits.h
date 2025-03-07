// This file is generated by TypeBuilder_h.template.

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef blink_protocol_Audits_h
#define blink_protocol_Audits_h

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "third_party/blink/renderer/core/inspector/protocol/Network.h"

namespace blink {
namespace protocol {
namespace Audits {

// ------------- Forward and enum declarations.

namespace GetEncodedResponse {
namespace EncodingEnum {
CORE_EXPORT extern const char* Webp;
CORE_EXPORT extern const char* Jpeg;
CORE_EXPORT extern const char* Png;
} // EncodingEnum
} // GetEncodedResponse

// ------------- Type and builder declarations.

// ------------- Backend interface.

class CORE_EXPORT Backend {
public:
    virtual ~Backend() { }

    virtual DispatchResponse getEncodedResponse(const String& in_requestId, const String& in_encoding, Maybe<double> in_quality, Maybe<bool> in_sizeOnly, Maybe<Binary>* out_body, int* out_originalSize, int* out_encodedSize) = 0;

    virtual DispatchResponse disable()
    {
        return DispatchResponse::OK();
    }
};

// ------------- Frontend interface.

class CORE_EXPORT Frontend {
public:
    explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }

    void flush();
    void sendRawJSONNotification(String);
    void sendRawCBORNotification(std::vector<uint8_t>);
private:
    FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class CORE_EXPORT Dispatcher {
public:
    static void wire(UberDispatcher*, Backend*);

private:
    Dispatcher() { }
};

// ------------- Metainfo.

class CORE_EXPORT Metainfo {
public:
    using BackendClass = Backend;
    using FrontendClass = Frontend;
    using DispatcherClass = Dispatcher;
    static const char domainName[];
    static const char commandPrefix[];
    static const char version[];
};

} // namespace Audits
} // namespace blink
} // namespace protocol

#endif // !defined(blink_protocol_Audits_h)
