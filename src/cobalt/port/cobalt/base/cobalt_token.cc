// Copyright 2016 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/base/cobalt_token.h"

#include <set>
#include <vector>

#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"
#include "base/synchronization/lock.h"

namespace base {

namespace {

// TODO: Replace more potential strings with Token, like event names and
//       attribute names.
// TODO: Turn kTagName of HTMLElements into Token.
// TODO: Implement Token friendly containers.
// TODO: Improve the internal storage of Token.  Possibly remove the use of
//       std::string and manage the buffer by ourselves.
class TokenStorage {
 public:
  static TokenStorage* GetInstance() {
    return base::Singleton<TokenStorage>::get();
  }

  const char* GetStorage(const char* str);

 private:
  friend struct base::DefaultSingletonTraits<TokenStorage>;

  TokenStorage()
      : hash_table_(CobToken::kHashSlotCount * CobToken::kStringsPerSlot) {}

  std::vector<std::string> hash_table_;
  // The collision table contains Tokens whose hash values collide with one of
  // the existing Token.  This should happen very rare.
  std::set<std::string> collision_table_;
  const std::string empty_;
  base::Lock lock_;
};

uint32 hash(const char* str) {
  return std::hash<std::string>()(std::string(str));
}

/*#if defined(BASE_HASH_USE_HASH_STRUCT)

uint32 hash(const char* str) {
  return BASE_HASH_NAMESPACE::hash<std::string>()(std::string(str));
}

#else

uint32 hash(const char* str) {
  return BASE_HASH_NAMESPACE::hash_value(std::string(str));
}

#endif  // COMPILER*/

}  // namespace

#ifdef ENABLE_TOKEN_ALPHABETICAL_SORTING
bool Token::sort_alphabetically_ = false;
#endif  // ENABLE_TOKEN_TEXT_SORTING

CobToken::CobToken() : str_(TokenStorage::GetInstance()->GetStorage("")) {}

CobToken::CobToken(const char* str)
    : str_(TokenStorage::GetInstance()->GetStorage(str)) {}

CobToken::CobToken(const std::string& str)
    : str_(TokenStorage::GetInstance()->GetStorage(str.c_str())) {}

const char* TokenStorage::GetStorage(const char* str) {
  DCHECK(str);

  if (!str || str[0] == 0) {
    return empty_.c_str();
  }

  uint32 slot = hash(str) % CobToken::kHashSlotCount;
  // First check if we already have this token.
  for (uint32 i = 0; i < CobToken::kStringsPerSlot; ++i) {
    uint32 index = slot * CobToken::kStringsPerSlot + i;
    if (hash_table_[index].empty()) {
      break;
    }
    if (hash_table_[index] == str) {
      return hash_table_[index].c_str();
    }
  }

  base::AutoLock auto_lock(lock_);
  // Now try to create this token in the table.
  for (uint32 i = 0; i < CobToken::kStringsPerSlot; ++i) {
    uint32 index = slot * CobToken::kStringsPerSlot + i;
    if (hash_table_[index].empty()) {
      hash_table_[index] = str;
      return hash_table_[index].c_str();
    }
  }

  // This should never happen in production as we use a quite large bin size
  // compares to the tokens in the system.  But it is does, we want to ensure
  // that the code can still work without crashing.
  if (collision_table_.empty()) {
    LOG(ERROR) << "Got collisions in Token.";
  }

  std::set<std::string>::iterator iter = collision_table_.find(str);
  if (iter != collision_table_.end()) {
    return iter->c_str();
  }
  return collision_table_.insert(str).first->c_str();
}

}  // namespace base
