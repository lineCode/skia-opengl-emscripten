// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/public/cpp/bindings/connector.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <utility>

#include "base/bind.h"
#include "base/callback.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/stl_util.h"
#include "base/threading/thread.h"
#include "base/threading/thread_task_runner_handle.h"
#include "mojo/public/cpp/bindings/message.h"
#include "mojo/public/cpp/bindings/tests/message_queue.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace mojo {
namespace test {
namespace {

class MessageAccumulator : public MessageReceiver {
 public:
  MessageAccumulator() {}
  explicit MessageAccumulator(const base::Closure& closure)
      : closure_(closure) {}

  bool Accept(Message* message) override {
    queue_.Push(message);
    if (!closure_.is_null())
      std::move(closure_).Run();
    return true;
  }

  bool IsEmpty() const { return queue_.IsEmpty(); }

  void Pop(Message* message) { queue_.Pop(message); }

  void set_closure(const base::Closure& closure) { closure_ = closure; }

  size_t size() const { return queue_.size(); }

 private:
  MessageQueue queue_;
  base::Closure closure_;
};

class ConnectorDeletingMessageAccumulator : public MessageAccumulator {
 public:
  ConnectorDeletingMessageAccumulator(Connector** connector)
      : connector_(connector) {}

  bool Accept(Message* message) override {
    delete *connector_;
    *connector_ = nullptr;
    return MessageAccumulator::Accept(message);
  }

 private:
  Connector** connector_;
};

class ReentrantMessageAccumulator : public MessageAccumulator {
 public:
  ReentrantMessageAccumulator(Connector* connector)
      : connector_(connector), number_of_calls_(0) {}

  bool Accept(Message* message) override {
    if (!MessageAccumulator::Accept(message))
      return false;
    number_of_calls_++;
    if (number_of_calls_ == 1) {
      return connector_->WaitForIncomingMessage(MOJO_DEADLINE_INDEFINITE);
    }
    return true;
  }

  int number_of_calls() { return number_of_calls_; }

 private:
  Connector* connector_;
  int number_of_calls_;
};

class ConnectorTest : public testing::Test {
 public:
  ConnectorTest() {}

  void SetUp() override { CreateMessagePipe(nullptr, &handle0_, &handle1_); }

  void TearDown() override {}

  Message CreateMessage(
      const char* text,
      std::vector<ScopedHandle> handles = std::vector<ScopedHandle>()) {
    const size_t size = strlen(text) + 1;  // Plus null terminator.
    Message message(1, 0, size, 0, &handles);
    memcpy(message.payload_buffer()->AllocateAndGet(size), text, size);
    return message;
  }

 protected:
  ScopedMessagePipeHandle handle0_;
  ScopedMessagePipeHandle handle1_;

 private:
  base::MessageLoop loop_;
};

TEST_F(ConnectorTest, Basic) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);
  connector0.Accept(&message);

  base::RunLoop run_loop;
  MessageAccumulator accumulator(run_loop.QuitClosure());
  connector1.set_incoming_receiver(&accumulator);

  run_loop.Run();

  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
}

TEST_F(ConnectorTest, Basic_Synchronous) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);
  connector0.Accept(&message);

  MessageAccumulator accumulator;
  connector1.set_incoming_receiver(&accumulator);

  connector1.WaitForIncomingMessage(MOJO_DEADLINE_INDEFINITE);

  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
}

TEST_F(ConnectorTest, Basic_EarlyIncomingReceiver) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  base::RunLoop run_loop;
  MessageAccumulator accumulator(run_loop.QuitClosure());
  connector1.set_incoming_receiver(&accumulator);

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);
  connector0.Accept(&message);

  run_loop.Run();

  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
}

TEST_F(ConnectorTest, Basic_TwoMessages) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char* kText[] = {"hello", "world"};
  for (size_t i = 0; i < base::size(kText); ++i) {
    Message message = CreateMessage(kText[i]);
    connector0.Accept(&message);
  }

  MessageAccumulator accumulator;
  connector1.set_incoming_receiver(&accumulator);

  for (size_t i = 0; i < base::size(kText); ++i) {
    if (accumulator.IsEmpty()) {
      base::RunLoop run_loop;
      accumulator.set_closure(run_loop.QuitClosure());
      run_loop.Run();
    }
    ASSERT_FALSE(accumulator.IsEmpty());

    Message message_received;
    accumulator.Pop(&message_received);

    EXPECT_EQ(
        std::string(kText[i]),
        std::string(reinterpret_cast<const char*>(message_received.payload())));
  }
}

TEST_F(ConnectorTest, Basic_TwoMessages_Synchronous) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char* kText[] = {"hello", "world"};
  for (size_t i = 0; i < base::size(kText); ++i) {
    Message message = CreateMessage(kText[i]);
    connector0.Accept(&message);
  }

  MessageAccumulator accumulator;
  connector1.set_incoming_receiver(&accumulator);

  connector1.WaitForIncomingMessage(MOJO_DEADLINE_INDEFINITE);

  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText[0]),
      std::string(reinterpret_cast<const char*>(message_received.payload())));

  ASSERT_TRUE(accumulator.IsEmpty());
}

TEST_F(ConnectorTest, WriteToClosedPipe) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);

  // Close the other end of the pipe.
  handle1_.reset();

  // Not observed yet because we haven't spun the message loop yet.
  EXPECT_FALSE(connector0.encountered_error());

  // Write failures are not reported.
  bool ok = connector0.Accept(&message);
  EXPECT_TRUE(ok);

  // Still not observed.
  EXPECT_FALSE(connector0.encountered_error());

  // Spin the message loop, and then we should start observing the closed pipe.
  base::RunLoop run_loop;
  connector0.set_connection_error_handler(run_loop.QuitClosure());
  run_loop.Run();

  EXPECT_TRUE(connector0.encountered_error());
}

TEST_F(ConnectorTest, MessageWithHandles) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";

  MessagePipe pipe;
  std::vector<ScopedHandle> handles;
  handles.emplace_back(ScopedHandle::From(std::move(pipe.handle0)));
  Message message1 = CreateMessage(kText, std::move(handles));

  connector0.Accept(&message1);

  base::RunLoop run_loop;
  MessageAccumulator accumulator(run_loop.QuitClosure());
  connector1.set_incoming_receiver(&accumulator);

  run_loop.Run();

  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
  ASSERT_EQ(1U, message_received.handles()->size());

  // Now send a message to the transferred handle and confirm it's sent through
  // to the orginal pipe.
  auto pipe_handle = ScopedMessagePipeHandle::From(
      std::move(message_received.mutable_handles()->front()));
  Connector connector_received(std::move(pipe_handle),
                               Connector::SINGLE_THREADED_SEND,
                               base::ThreadTaskRunnerHandle::Get());
  Connector connector_original(std::move(pipe.handle1),
                               Connector::SINGLE_THREADED_SEND,
                               base::ThreadTaskRunnerHandle::Get());

  Message message2 = CreateMessage(kText);
  connector_received.Accept(&message2);
  base::RunLoop run_loop2;
  MessageAccumulator accumulator2(run_loop2.QuitClosure());
  connector_original.set_incoming_receiver(&accumulator2);
  run_loop2.Run();

  ASSERT_FALSE(accumulator2.IsEmpty());

  accumulator2.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
}

TEST_F(ConnectorTest, WaitForIncomingMessageWithError) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  // Close the other end of the pipe.
  handle1_.reset();
  ASSERT_FALSE(connector0.WaitForIncomingMessage(MOJO_DEADLINE_INDEFINITE));
}

TEST_F(ConnectorTest, WaitForIncomingMessageWithDeletion) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector* connector1 =
      new Connector(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                    base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);
  connector0.Accept(&message);

  ConnectorDeletingMessageAccumulator accumulator(&connector1);
  connector1->set_incoming_receiver(&accumulator);

  connector1->WaitForIncomingMessage(MOJO_DEADLINE_INDEFINITE);

  ASSERT_FALSE(connector1);
  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));
}

TEST_F(ConnectorTest, WaitForIncomingMessageWithReentrancy) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char* kText[] = {"hello", "world"};
  for (size_t i = 0; i < base::size(kText); ++i) {
    Message message = CreateMessage(kText[i]);
    connector0.Accept(&message);
  }

  ReentrantMessageAccumulator accumulator(&connector1);
  connector1.set_incoming_receiver(&accumulator);

  for (size_t i = 0; i < base::size(kText); ++i) {
    if (accumulator.IsEmpty()) {
      base::RunLoop run_loop;
      accumulator.set_closure(run_loop.QuitClosure());
      run_loop.Run();
    }
    ASSERT_FALSE(accumulator.IsEmpty());

    Message message_received;
    accumulator.Pop(&message_received);

    EXPECT_EQ(
        std::string(kText[i]),
        std::string(reinterpret_cast<const char*>(message_received.payload())));
  }

  ASSERT_EQ(2, accumulator.number_of_calls());
}

void ForwardErrorHandler(bool* called, const base::Closure& callback) {
  *called = true;
  callback.Run();
}

TEST_F(ConnectorTest, RaiseError) {
  base::RunLoop run_loop, run_loop2;
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  bool error_handler_called0 = false;
  connector0.set_connection_error_handler(base::Bind(
      &ForwardErrorHandler, &error_handler_called0, run_loop.QuitClosure()));

  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  bool error_handler_called1 = false;
  connector1.set_connection_error_handler(base::Bind(
      &ForwardErrorHandler, &error_handler_called1, run_loop2.QuitClosure()));

  const char kText[] = "hello world";
  Message message = CreateMessage(kText);
  connector0.Accept(&message);
  connector0.RaiseError();

  base::RunLoop run_loop3;
  MessageAccumulator accumulator(run_loop3.QuitClosure());
  connector1.set_incoming_receiver(&accumulator);

  run_loop3.Run();

  // Messages sent prior to RaiseError() still arrive at the other end.
  ASSERT_FALSE(accumulator.IsEmpty());

  Message message_received;
  accumulator.Pop(&message_received);

  EXPECT_EQ(
      std::string(kText),
      std::string(reinterpret_cast<const char*>(message_received.payload())));

  run_loop.Run();
  run_loop2.Run();

  // Connection error handler is called at both sides.
  EXPECT_TRUE(error_handler_called0);
  EXPECT_TRUE(error_handler_called1);

  // The error flag is set at both sides.
  EXPECT_TRUE(connector0.encountered_error());
  EXPECT_TRUE(connector1.encountered_error());

  // The message pipe handle is valid at both sides.
  EXPECT_TRUE(connector0.is_valid());
  EXPECT_TRUE(connector1.is_valid());
}

void PauseConnectorAndRunClosure(Connector* connector,
                                 const base::Closure& closure) {
  connector->PauseIncomingMethodCallProcessing();
  closure.Run();
}

TEST_F(ConnectorTest, PauseWithQueuedMessages) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";

  // Queue up two messages.
  Message message = CreateMessage(kText);
  connector0.Accept(&message);
  message = CreateMessage(kText);
  connector0.Accept(&message);

  base::RunLoop run_loop;
  // Configure the accumulator such that it pauses after the first message is
  // received.
  MessageAccumulator accumulator(base::Bind(
      &PauseConnectorAndRunClosure, &connector1, run_loop.QuitClosure()));
  connector1.set_incoming_receiver(&accumulator);

  run_loop.Run();

  // As we paused after the first message we should only have gotten one
  // message.
  ASSERT_EQ(1u, accumulator.size());
}

void AccumulateWithNestedLoop(MessageAccumulator* accumulator,
                              const base::Closure& closure) {
  base::RunLoop nested_run_loop(base::RunLoop::Type::kNestableTasksAllowed);
  accumulator->set_closure(nested_run_loop.QuitClosure());
  nested_run_loop.Run();
  closure.Run();
}

TEST_F(ConnectorTest, ProcessWhenNested) {
  Connector connector0(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());
  Connector connector1(std::move(handle1_), Connector::SINGLE_THREADED_SEND,
                       base::ThreadTaskRunnerHandle::Get());

  const char kText[] = "hello world";

  // Queue up two messages.
  Message message = CreateMessage(kText);
  connector0.Accept(&message);
  message = CreateMessage(kText);
  connector0.Accept(&message);

  base::RunLoop run_loop;
  MessageAccumulator accumulator;
  // When the accumulator gets the first message it spins a nested message
  // loop. The loop is quit when another message is received.
  accumulator.set_closure(base::Bind(&AccumulateWithNestedLoop, &accumulator,
                                     run_loop.QuitClosure()));
  connector1.set_incoming_receiver(&accumulator);

  run_loop.Run();

  ASSERT_EQ(2u, accumulator.size());
}

TEST_F(ConnectorTest, DestroyOnDifferentThreadAfterClose) {
  std::unique_ptr<Connector> connector(
      new Connector(std::move(handle0_), Connector::SINGLE_THREADED_SEND,
                    base::ThreadTaskRunnerHandle::Get()));

  connector->CloseMessagePipe();

  base::Thread another_thread("ThreadForDestroyingConnector");
  another_thread.Start();

  base::RunLoop run_loop;
  another_thread.task_runner()->PostTaskAndReply(
      FROM_HERE,
      base::Bind(
          [](std::unique_ptr<Connector> connector) { connector.reset(); },
          base::Passed(std::move(connector))),
      run_loop.QuitClosure());

  run_loop.Run();
}

}  // namespace
}  // namespace test
}  // namespace mojo
