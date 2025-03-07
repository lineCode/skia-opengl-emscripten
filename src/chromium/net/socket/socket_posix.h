// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKET_POSIX_H_
#define NET_SOCKET_SOCKET_POSIX_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_pump_for_io.h"
#include "base/threading/thread_checker.h"
#include "net/base/completion_once_callback.h"
#include "net/base/net_export.h"
#include "net/socket/socket_descriptor.h"
#include "net/traffic_annotation/network_traffic_annotation.h"

namespace net {

class IOBuffer;
struct SockaddrStorage;

#if !defined(OS_EMSCRIPTEN)
// Socket class to provide asynchronous read/write operations on top of the
// posix socket api. It supports AF_INET, AF_INET6, and AF_UNIX addresses.
class NET_EXPORT_PRIVATE SocketPosix
    : public base::MessagePumpForIO::FdWatcher {
 public:
  SocketPosix();
  ~SocketPosix() override;

  // Opens a socket and returns net::OK if |address_family| is AF_INET, AF_INET6
  // or AF_UNIX. Otherwise, it does DCHECK() and returns a net error.
  int Open(int address_family);

  // Takes ownership of |socket|, which is known to already be connected to the
  // given peer address.
  int AdoptConnectedSocket(SocketDescriptor socket,
                           const SockaddrStorage& peer_address);
  // Takes ownership of |socket|, which may or may not be open, bound, or
  // listening. The caller must determine the state of the socket based on its
  // provenance and act accordingly. The socket may have connections waiting
  // to be accepted, but must not be actually connected.
  int AdoptUnconnectedSocket(SocketDescriptor socket);

  // Releases ownership of |socket_fd_| to caller.
  SocketDescriptor ReleaseConnectedSocket();

  int Bind(const SockaddrStorage& address);

  int Listen(int backlog);
  int Accept(std::unique_ptr<SocketPosix>* socket,
             CompletionOnceCallback callback);

  // Connects socket. On non-ERR_IO_PENDING error, sets errno and returns a net
  // error code. On ERR_IO_PENDING, |callback| is called with a net error code,
  // not errno, though errno is set if connect event happens with error.
  // TODO(byungchul): Need more robust way to pass system errno.
  int Connect(const SockaddrStorage& address, CompletionOnceCallback callback);
  bool IsConnected() const;
  bool IsConnectedAndIdle() const;

  // Multiple outstanding requests of the same type are not supported.
  // Full duplex mode (reading and writing at the same time) is supported.
  // On error which is not ERR_IO_PENDING, sets errno and returns a net error
  // code. On ERR_IO_PENDING, |callback| is called with a net error code, not
  // errno, though errno is set if read or write events happen with error.
  // TODO(byungchul): Need more robust way to pass system errno.
  int Read(IOBuffer* buf, int buf_len, CompletionOnceCallback callback);

  // Reads up to |buf_len| bytes into |buf| without blocking. If read is to
  // be retried later, |callback| will be invoked when data is ready for
  // reading. This method doesn't hold on to |buf|.
  // See socket.h for more information.
  int ReadIfReady(IOBuffer* buf, int buf_len, CompletionOnceCallback callback);
  int CancelReadIfReady();
  int Write(IOBuffer* buf,
            int buf_len,
            CompletionOnceCallback callback,
            const NetworkTrafficAnnotationTag& traffic_annotation);

  // Waits for next write event. This is called by TCPSocketPosix for TCP
  // fastopen after sending first data. Returns ERR_IO_PENDING if it starts
  // waiting for write event successfully. Otherwise, returns a net error code.
  // It must not be called after Write() because Write() calls it internally.
  int WaitForWrite(IOBuffer* buf, int buf_len, CompletionOnceCallback callback);

  int GetLocalAddress(SockaddrStorage* address) const;
  int GetPeerAddress(SockaddrStorage* address) const;
  void SetPeerAddress(const SockaddrStorage& address);
  // Returns true if peer address has been set regardless of socket state.
  bool HasPeerAddress() const;

  void Close();

  // Detachs from the current thread, to allow the socket to be transferred to
  // a new thread. Should only be called when the object is no longer used by
  // the old thread.
  void DetachFromThread();

  SocketDescriptor socket_fd() const { return socket_fd_; }

 private:
  // base::MessagePumpForIO::FdWatcher methods.
  void OnFileCanReadWithoutBlocking(int fd) override;
  void OnFileCanWriteWithoutBlocking(int fd) override;

  int DoAccept(std::unique_ptr<SocketPosix>* socket);
  void AcceptCompleted();

  int DoConnect();
  void ConnectCompleted();

  int DoRead(IOBuffer* buf, int buf_len);
  void RetryRead(int rv);
  void ReadCompleted();

  int DoWrite(IOBuffer* buf, int buf_len);
  void WriteCompleted();

  void StopWatchingAndCleanUp();

  SocketDescriptor socket_fd_;

#if !defined(OS_EMSCRIPTEN)
  base::MessagePumpForIO::FdWatchController accept_socket_watcher_;
#endif

  std::unique_ptr<SocketPosix>* accept_socket_;
  CompletionOnceCallback accept_callback_;

#if !defined(OS_EMSCRIPTEN)
  base::MessagePumpForIO::FdWatchController read_socket_watcher_;
#endif

  // Non-null when a Read() is in progress.
  scoped_refptr<IOBuffer> read_buf_;
  int read_buf_len_;
  CompletionOnceCallback read_callback_;

  // Non-null when a ReadIfReady() is in progress.
  CompletionOnceCallback read_if_ready_callback_;

#if !defined(OS_EMSCRIPTEN)
  base::MessagePumpForIO::FdWatchController write_socket_watcher_;
#endif

  scoped_refptr<IOBuffer> write_buf_;
  int write_buf_len_;
  // External callback; called when write or connect is complete.
  CompletionOnceCallback write_callback_;

  // A connect operation is pending. In this case, |write_callback_| needs to be
  // called when connect is complete.
  bool waiting_connect_;

  std::unique_ptr<SockaddrStorage> peer_address_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(SocketPosix);
};
#endif

}  // namespace net

#endif  // NET_SOCKET_SOCKET_POSIX_H_
