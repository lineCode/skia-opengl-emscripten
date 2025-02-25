// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/url_request/url_request_netlog_params.h"

#include <utility>

#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "net/log/net_log_capture_mode.h"
#include "url/gurl.h"

namespace net {

base::Value NetLogURLRequestConstructorCallback(
    const GURL* url,
    RequestPriority priority,
    NetworkTrafficAnnotationTag traffic_annotation,
    NetLogCaptureMode /* capture_mode */) {
  base::DictionaryValue dict;
  dict.SetString("url", url->possibly_invalid_spec());
  dict.SetString("priority", RequestPriorityToString(priority));
  dict.SetInteger("traffic_annotation", traffic_annotation.unique_id_hash_code);
  return std::move(dict);
}

base::Value NetLogURLRequestStartCallback(
    const GURL* url,
    const std::string* method,
    int load_flags,
    PrivacyMode privacy_mode,
    int64_t upload_id,
    NetLogCaptureMode /* capture_mode */) {
  base::DictionaryValue dict;
  dict.SetString("url", url->possibly_invalid_spec());
  dict.SetString("method", *method);
  dict.SetInteger("load_flags", load_flags);
  dict.SetInteger("privacy_mode", privacy_mode == PRIVACY_MODE_ENABLED);
  if (upload_id > -1)
    dict.SetString("upload_id", base::NumberToString(upload_id));
  return std::move(dict);
}

}  // namespace net
