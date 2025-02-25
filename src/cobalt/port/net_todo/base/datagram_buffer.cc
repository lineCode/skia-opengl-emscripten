// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/datagram_buffer.h"
#include "net/third_party/quic/platform/api/quic_ptr_util.h"

#include "starboard/memory.h"

namespace net {

DatagramBufferPool::DatagramBufferPool(size_t max_buffer_size)
    : max_buffer_size_(max_buffer_size) {}

DatagramBufferPool::~DatagramBufferPool() {}

void DatagramBufferPool::Enqueue(const char* buffer,
                                 size_t buf_len,
                                 DatagramBuffers* buffers) {
  DCHECK_LE(buf_len, max_buffer_size_);
  std::unique_ptr<DatagramBuffer> datagram_buffer;
  if (free_list_.empty()) {
    datagram_buffer = quic::QuicWrapUnique<DatagramBuffer>(
        new DatagramBuffer(max_buffer_size_));
  } else {
    datagram_buffer = std::move(free_list_.front());
    free_list_.pop_front();
  }
  datagram_buffer->Set(buffer, buf_len);
  buffers->emplace_back(std::move(datagram_buffer));
}

void DatagramBufferPool::Dequeue(DatagramBuffers* buffers) {
  if (buffers->size() == 0)
    return;

#if defined(STARBOARD)
  free_list_.splice(free_list_.end(), *buffers);
#else
  free_list_.splice(free_list_.cend(), *buffers);
#endif
}

DatagramBuffer::DatagramBuffer(size_t max_buffer_size)
    : data_(new char[max_buffer_size]), length_(0) {}

DatagramBuffer::~DatagramBuffer() {}

void DatagramBuffer::Set(const char* buffer, size_t buf_len) {
  length_ = buf_len;
  SbMemoryCopy(data_.get(), buffer, buf_len);
}

char* DatagramBuffer::data() const {
  return data_.get();
}

size_t DatagramBuffer::length() const {
  return length_;
}

}  // namespace net
