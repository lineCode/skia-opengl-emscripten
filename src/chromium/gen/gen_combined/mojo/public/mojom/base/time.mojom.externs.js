// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



goog.provide('mojoBase.mojom.Time');

mojoBase.mojom.Time = class {
  constructor() {
    /** @type { !number } */
    this.internalValue;
  }
};

goog.provide('mojoBase.mojom.TimeDelta');

mojoBase.mojom.TimeDelta = class {
  constructor() {
    /** @type { !number } */
    this.microseconds;
  }
};

goog.provide('mojoBase.mojom.TimeTicks');

mojoBase.mojom.TimeTicks = class {
  constructor() {
    /** @type { !number } */
    this.internalValue;
  }
};
