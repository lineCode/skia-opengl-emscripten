// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_METADATA_PROPERTY_METADATA_H_
#define UI_VIEWS_METADATA_PROPERTY_METADATA_H_

#include <string>
#include <type_traits>

#include "base/macros.h"
#include "base/strings/string16.h"
#include "ui/views/metadata/metadata_cache.h"
#include "ui/views/metadata/type_conversion.h"
#include "ui/views/views_export.h"

namespace views {
namespace metadata {

// Represents meta data for a specific read-only property member of class
// |TClass|, with underlying type |TValue|, as the type of the actual member.
template <typename TClass, typename TValue, TValue (TClass::*Get)() const>
class ClassPropertyReadOnlyMetaData : public MemberMetaDataBase {
 public:
  ClassPropertyReadOnlyMetaData() {}
  ~ClassPropertyReadOnlyMetaData() override = default;

  base::string16 GetValueAsString(void* obj) const override {
    return Convert<TValue, base::string16>((static_cast<TClass*>(obj)->*Get)());
  }

  PropertyFlags GetPropertyFlags() const override {
    return PropertyFlags::kReadOnly;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ClassPropertyReadOnlyMetaData);
};

// Represents meta data for a specific property member of class |TClass|, with
// underlying type |TValue|, as the type of the actual member.
// Allows for interaction with the property as if it were the underlying data
// type (|TValue|), but still uses the Property's functionality under the hood
// (so it will trigger things like property changed notifications).
template <typename TClass,
          typename TValue,
          void (TClass::*Set)(ArgType<TValue>),
          TValue (TClass::*Get)() const>
class ClassPropertyMetaData
    : public ClassPropertyReadOnlyMetaData<TClass, TValue, Get> {
 public:
  ClassPropertyMetaData() {}
  ~ClassPropertyMetaData() override = default;

  void SetValueAsString(void* obj, const base::string16& new_value) override {
    (static_cast<TClass*>(obj)->*Set)(Convert<base::string16, TValue>(
        new_value, (static_cast<TClass*>(obj)->*Get)()));
  }

  PropertyFlags GetPropertyFlags() const override {
    return PropertyFlags::kEmpty;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ClassPropertyMetaData);
};

}  // namespace metadata
}  // namespace views

#endif  // UI_VIEWS_METADATA_PROPERTY_METADATA_H_
