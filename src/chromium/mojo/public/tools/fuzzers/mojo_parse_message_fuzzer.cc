// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/task/thread_pool/thread_pool.h"
#include "mojo/core/embedder/embedder.h"
#include "mojo/public/cpp/bindings/binding.h"
#include "mojo/public/tools/fuzzers/fuzz_impl.h"

void FuzzMessage(const uint8_t* data, size_t size, base::RunLoop* run) {
  fuzz::mojom::FuzzInterfacePtr fuzz;
  auto impl = std::make_unique<FuzzImpl>(MakeRequest(&fuzz));
  auto router = impl->binding_.RouterForTesting();

  /* Create a mojo message with the appropriate payload size. */
  mojo::Message message(0, 0, size, 0, nullptr);
  if (message.data_num_bytes() < size) {
    message.payload_buffer()->Allocate(size - message.data_num_bytes());
  }

  /* Set the raw message data. */
  memcpy(message.mutable_data(), data, size);

  /* Run the message through header validation, payload validation, and
   * dispatch to the impl. */
  router->SimulateReceivingMessageForTesting(&message);

  /* Allow the harness function to return now. */
  run->Quit();
}

/* Environment for the fuzzer. Initializes the mojo EDK and sets up a
 * ThreadPool, because Mojo messages must be sent and processed from
 * TaskRunners. */
struct Environment {
  Environment() : message_loop(base::MessageLoop::TYPE_UI) {
    base::ThreadPool::CreateAndStartWithDefaultParams(
        "MojoParseMessageFuzzerProcess");
    mojo::core::Init();
  }

  /* Message loop to send and handle messages on. */
  base::MessageLoop message_loop;

  /* Suppress mojo validation failure logs. */
  mojo::internal::ScopedSuppressValidationErrorLoggingForTests log_suppression;
};

// Entry point for LibFuzzer.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  static Environment* env = new Environment();
  /* Pass the data along to run on a MessageLoop, and wait for it to finish. */
  base::RunLoop run;
  env->message_loop.task_runner()->PostTask(
      FROM_HERE, base::BindOnce(&FuzzMessage, data, size, &run));
  run.Run();

  return 0;
}
