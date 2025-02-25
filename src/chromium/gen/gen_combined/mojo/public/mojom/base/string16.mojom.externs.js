// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



goog.provide('mojoBase.mojom.String16');

mojoBase.mojom.String16 = class {
  constructor() {
    /** @type { !Array<number> } */
    this.data;
  }
};

goog.provide('mojoBase.mojom.BigString16');

mojoBase.mojom.BigString16 = class {
  constructor() {
    /** @type { !Object } */
    this.data;
  }
};
