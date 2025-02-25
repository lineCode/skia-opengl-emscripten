// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/streams/transform_stream_native.h"

#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/core/streams/miscellaneous_operations.h"
#include "third_party/blink/renderer/core/streams/promise_handler.h"
#include "third_party/blink/renderer/core/streams/readable_stream_default_controller.h"
#include "third_party/blink/renderer/core/streams/readable_stream_native.h"
#include "third_party/blink/renderer/core/streams/stream_algorithms.h"
#include "third_party/blink/renderer/core/streams/stream_promise_resolver.h"
#include "third_party/blink/renderer/core/streams/transform_stream_default_controller.h"
#include "third_party/blink/renderer/core/streams/transform_stream_default_controller_interface.h"
#include "third_party/blink/renderer/core/streams/transform_stream_transformer.h"
#include "third_party/blink/renderer/core/streams/writable_stream_default_controller.h"
#include "third_party/blink/renderer/core/streams/writable_stream_native.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/platform/bindings/to_v8.h"
#include "third_party/blink/renderer/platform/bindings/v8_binding.h"
#include "third_party/blink/renderer/platform/heap/visitor.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

// The controller that is passed to TransformStreamTransformer implementations
// is a stack-allocated wrapper of the real controller. This gives interface
// compatibility with the V8 Extras implementation.
class TransformStreamNative::ControllerInterface final
    : public TransformStreamDefaultControllerInterface {
  STACK_ALLOCATED();

 public:
  ControllerInterface(ScriptState* script_state,
                      TransformStreamDefaultController* controller)
      : script_state_(script_state), controller_(controller) {}

  void Enqueue(v8::Local<v8::Value> chunk,
               ExceptionState& exception_state) override {
    TransformStreamDefaultController::Enqueue(script_state_, controller_, chunk,
                                              exception_state);
  }

 private:
  Member<ScriptState> script_state_;
  Member<TransformStreamDefaultController> controller_;
};

// Implements a flush algorithm that delegates to a TransformStreamTransformer.
// This is used when creating a TransformStream from C++.
class TransformStreamNative::FlushAlgorithm final : public StreamAlgorithm {
 public:
  explicit FlushAlgorithm(TransformStreamTransformer* transformer)
      : transformer_(transformer) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 0);
    DCHECK(controller_);
    ExceptionState exception_state(script_state->GetIsolate(),
                                   ExceptionState::kUnknownContext, "", "");
    ControllerInterface controller_interface(script_state, controller_);
    transformer_->Flush(&controller_interface, exception_state);
    if (exception_state.HadException()) {
      auto exception = exception_state.GetException();
      exception_state.ClearException();
      return PromiseReject(script_state, exception);
    }

    return PromiseResolveWithUndefined(script_state);
  }

  // SetController() must be called before Run() is.
  void SetController(TransformStreamDefaultController* controller) {
    controller_ = controller;
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(transformer_);
    visitor->Trace(controller_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamTransformer> transformer_;
  Member<TransformStreamDefaultController> controller_;
};

// Implements a transform algorithm that delegates to a
// TransformStreamTransformer.
class TransformStreamNative::TransformAlgorithm final : public StreamAlgorithm {
 public:
  explicit TransformAlgorithm(TransformStreamTransformer* transformer)
      : transformer_(transformer) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 1);
    DCHECK(controller_);
    ExceptionState exception_state(script_state->GetIsolate(),
                                   ExceptionState::kUnknownContext, "", "");
    ControllerInterface controller_interface(script_state, controller_);
    transformer_->Transform(argv[0], &controller_interface, exception_state);
    if (exception_state.HadException()) {
      auto exception = exception_state.GetException();
      exception_state.ClearException();
      return PromiseReject(script_state, exception);
    }

    return PromiseResolveWithUndefined(script_state);
  }

  // SetController() must be called before Run() is.
  void SetController(TransformStreamDefaultController* controller) {
    controller_ = controller;
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(transformer_);
    visitor->Trace(controller_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamTransformer> transformer_;
  Member<TransformStreamDefaultController> controller_;
};

// static
void TransformStreamNative::InitFromJS(ScriptState* script_state,
                                       ScriptValue raw_transformer,
                                       ScriptValue raw_writable_strategy,
                                       ScriptValue raw_readable_strategy,
                                       Member<ReadableStream>* readable,
                                       Member<WritableStream>* writable,
                                       ExceptionState& exception_state) {
  const auto* stream = MakeGarbageCollected<TransformStreamNative>(
      script_state, raw_transformer, raw_writable_strategy,
      raw_readable_strategy, exception_state);
  *readable = stream->readable_;
  *writable = stream->writable_;
  // We no longer need a direct reference to |stream|.
}

// static
void TransformStreamNative::Init(ScriptState* script_state,
                                 TransformStreamTransformer* transformer,
                                 Member<ReadableStream>* readable,
                                 Member<WritableStream>* writable,
                                 ExceptionState& exception_state) {
  auto* transform_algorithm =
      MakeGarbageCollected<TransformAlgorithm>(transformer);
  auto* flush_algorithm = MakeGarbageCollected<FlushAlgorithm>(transformer);
  auto* size_algorithm = CreateDefaultSizeAlgorithm();
  auto* stream = Create(script_state, CreateTrivialStartAlgorithm(),
                        transform_algorithm, flush_algorithm, 1, size_algorithm,
                        0, size_algorithm, exception_state);
  DCHECK(stream);
  DCHECK(!exception_state.HadException());
  TransformStreamDefaultController* controller =
      stream->transform_stream_controller_;
  transform_algorithm->SetController(controller);
  flush_algorithm->SetController(controller);
  *readable = stream->readable_;
  *writable = stream->writable_;
  // We no longer need a direct reference to |stream|.
}

TransformStreamNative* TransformStreamNative::Create(
    ScriptState* script_state,
    StreamStartAlgorithm* start_algorithm,
    StreamAlgorithm* transform_algorithm,
    StreamAlgorithm* flush_algorithm,
    double writable_high_water_mark,
    StrategySizeAlgorithm* writable_size_algorithm,
    double readable_high_water_mark,
    StrategySizeAlgorithm* readable_size_algorithm,
    ExceptionState& exception_state) {
  // https://streams.spec.whatwg.org/#create-transform-stream
  //
  // In this implementation, all the arguments are compulsory, so the following
  // steps aren't performed:
  // 1. If writableHighWaterMark was not passed, set it to 1.
  // 2. If writableSizeAlgorithm was not passed, set it to an algorithm that
  //    returns 1.
  // 3. If readableHighWaterMark was not passed, set it to 0.
  // 4. If readableSizeAlgorithm was not passed, set it to an algorithm that
  //    returns 1.

  // 5. Assert: ! IsNonNegativeNumber(writableHighWaterMark) is true.
  DCHECK_GE(writable_high_water_mark, 0);

  // 6. Assert: ! IsNonNegativeNumber(readableHighWaterMark) is true.
  DCHECK_GE(readable_high_water_mark, 0);

  // 7. Let stream be ObjectCreate(the original value of TransformStream's
  //    prototype property).
  auto* stream = MakeGarbageCollected<TransformStreamNative>();

  // 8. Let startPromise be a new promise.
  auto* start_promise =
      MakeGarbageCollected<StreamPromiseResolver>(script_state);

  // 9. Perform ! InitializeTransformStream(stream, startPromise,
  //    writableHighWaterMark, writableSizeAlgorithm, readableHighWaterMark,
  //    readableSizeAlgorithm).
  Initialize(script_state, stream, start_promise, writable_high_water_mark,
             writable_size_algorithm, readable_high_water_mark,
             readable_size_algorithm);

  // 10. Let controller be ObjectCreate(the original value of
  //     TransformStreamDefaultController's prototype property).
  auto* controller = MakeGarbageCollected<TransformStreamDefaultController>();

  // 11. Perform ! SetUpTransformStreamDefaultController(stream, controller,
  //     transformAlgorithm, flushAlgorithm).
  TransformStreamDefaultController::SetUp(stream, controller,
                                          transform_algorithm, flush_algorithm);

  // 12. Let startResult be the result of performing startAlgorithm. (This may
  //     throw an exception.)
  v8::MaybeLocal<v8::Promise> start_result_maybe =
      start_algorithm->Run(script_state, exception_state);
  v8::Local<v8::Promise> start_result;
  if (!start_result_maybe.ToLocal(&start_result)) {
    CHECK(exception_state.HadException());
    return nullptr;
  }
  DCHECK(!exception_state.HadException());

  // 13. Resolve startPromise with startResult.
  start_promise->Resolve(script_state, start_result);

  // 14. Return stream.
  return stream;
}

// This constructor is only used internally.
TransformStreamNative::TransformStreamNative() = default;

TransformStreamNative::TransformStreamNative(ScriptState* script_state,
                                             ScriptValue raw_transformer,
                                             ScriptValue raw_writable_strategy,
                                             ScriptValue raw_readable_strategy,
                                             ExceptionState& exception_state) {
  // TODO(ricea): Move this to IDL.
  UseCounter::Count(ExecutionContext::From(script_state),
                    WebFeature::kTransformStreamConstructor);

  DCHECK(!raw_transformer.IsEmpty());
  DCHECK(!raw_writable_strategy.IsEmpty());
  DCHECK(!raw_readable_strategy.IsEmpty());

  auto context = script_state->GetContext();
  auto* isolate = script_state->GetIsolate();

  // https://streams.spec.whatwg.org/#ts-constructor
  // Perform the "transformer = {}" step from the function signature.
  v8::Local<v8::Object> transformer;
  ScriptValueToObject(script_state, raw_transformer, &transformer,
                      exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // Perform the "writableStrategy = {}" step from the function signature, and
  // 1. Let writableSizeFunction be ? GetV(writableStrategy, "size").
  // 2. Let writableHighWaterMark be ? GetV(writableStrategy, "highWaterMark").
  StrategyUnpacker writable_strategy_unpacker(
      script_state, raw_writable_strategy, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // Perform the "readableStrategy = {}" step from the function signature, and
  // 3. Let readableSizeFunction be ? GetV(readableStrategy, "size").
  // 4. Let readableHighWaterMark be ? GetV(readableStrategy, "highWaterMark").
  StrategyUnpacker readable_strategy_unpacker(
      script_state, raw_readable_strategy, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  v8::TryCatch try_catch(isolate);

  // 5. Let writableType be ? GetV(transformer, "writableType").
  v8::Local<v8::Value> writable_type;
  if (!transformer->Get(context, V8AtomicString(isolate, "writableType"))
           .ToLocal(&writable_type)) {
    exception_state.RethrowV8Exception(try_catch.Exception());
    return;
  }

  // 6. If writableType is not undefined, throw a RangeError exception.
  if (!writable_type->IsUndefined()) {
    exception_state.ThrowRangeError("Invalid writableType was specified");
    return;
  }

  // 7. Let writableSizeAlgorithm be ? MakeSizeAlgorithmFromSizeFunction(
  //    writableSizeFunction).
  auto* writable_size_algorithm = writable_strategy_unpacker.MakeSizeAlgorithm(
      script_state, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // 8. If writableHighWaterMark is undefined, set writableHighWaterMark to 1.
  // 9. Set writableHighWaterMark to ? ValidateAndNormalizeHighWaterMark(
  //    writableHighWaterMark).
  double writable_high_water_mark = writable_strategy_unpacker.GetHighWaterMark(
      script_state, 1, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // 10. Let readableType be ? GetV(transformer, "readableType").
  v8::Local<v8::Value> readable_type;
  if (!transformer->Get(context, V8AtomicString(isolate, "readableType"))
           .ToLocal(&readable_type)) {
    exception_state.RethrowV8Exception(try_catch.Exception());
    return;
  }

  // 11. If readableType is not undefined, throw a RangeError exception.
  if (!readable_type->IsUndefined()) {
    exception_state.ThrowRangeError("Invalid readableType was specified");
    return;
  }

  // 12. Let readableSizeAlgorithm be ? MakeSizeAlgorithmFromSizeFunction(
  //     readableSizeFunction).
  auto* readable_size_algorithm = readable_strategy_unpacker.MakeSizeAlgorithm(
      script_state, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // 13. If readableHighWaterMark is undefined, set readableHighWaterMark to 0.
  // 14. Set readableHighWaterMark be ? ValidateAndNormalizeHighWaterMark(
  //     readableHighWaterMark).
  double readable_high_water_mark = readable_strategy_unpacker.GetHighWaterMark(
      script_state, 0, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // 15. Let startPromise be a new promise.
  auto* start_promise =
      MakeGarbageCollected<StreamPromiseResolver>(script_state);

  // 16. Perform ! InitializeTransformStream(this, startPromise,
  //     writableHighWaterMark, writableSizeAlgorithm, readableHighWaterMark,
  //     readableSizeAlgorithm).
  Initialize(script_state, this, start_promise, writable_high_water_mark,
             writable_size_algorithm, readable_high_water_mark,
             readable_size_algorithm);

  // 17. Perform ? SetUpTransformStreamDefaultControllerFromTransformer(this,
  //     transformer).
  const auto controller_value =
      TransformStreamDefaultController::SetUpFromTransformer(
          script_state, this, transformer, exception_state);
  if (exception_state.HadException()) {
    return;
  }

  // 18. Let startResult be ? InvokeOrNoop(transformer, "start", « this.
  //     [[transformStreamController]] »).
  v8::MaybeLocal<v8::Value> start_result_maybe =
      CallOrNoop1(script_state, transformer, "start", "transformer.start",
                  controller_value, exception_state);
  v8::Local<v8::Value> start_result;
  if (!start_result_maybe.ToLocal(&start_result)) {
    CHECK(exception_state.HadException());
    return;
  }
  DCHECK(!exception_state.HadException());

  // 19. Resolve startPromise with startResult.
  start_promise->Resolve(script_state, start_result);
}

void TransformStreamNative::Trace(Visitor* visitor) {
  visitor->Trace(backpressure_change_promise_);
  visitor->Trace(readable_);
  visitor->Trace(transform_stream_controller_);
  visitor->Trace(writable_);
}

// Implements the "an algorithm that returns startPromise" step from
// InitializeTransformStream():
// https://streams.spec.whatwg.org/#initialize-transform-stream.
class TransformStreamNative::ReturnStartPromiseAlgorithm final
    : public StreamStartAlgorithm {
 public:
  explicit ReturnStartPromiseAlgorithm(StreamPromiseResolver* start_promise)
      : start_promise_(start_promise) {}

  v8::MaybeLocal<v8::Promise> Run(ScriptState* script_state,
                                  ExceptionState&) override {
    return start_promise_->V8Promise(script_state->GetIsolate());
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(start_promise_);
    StreamStartAlgorithm::Trace(visitor);
  }

 private:
  Member<StreamPromiseResolver> start_promise_;
};

//
// The following algorithms are described as abstract operations in the
// standard, but need to be implemented as classes in C++ so that the data they
// store can be traced by the garbage collector correctly.
//
class TransformStreamNative::DefaultSinkWriteAlgorithm final
    : public StreamAlgorithm {
 public:
  explicit DefaultSinkWriteAlgorithm(TransformStreamNative* stream)
      : stream_(stream) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 1);
    const auto chunk = argv[0];

    // https://streams.spec.whatwg.org/#transform-stream-default-sink-write-algorithm
    // 1. Assert: stream.[[writable]].[[state]] is "writable".
    DCHECK(stream_->writable_->IsWritable());

    // 2. Let controller be stream.[[transformStreamController]].
    TransformStreamDefaultController* controller =
        stream_->transform_stream_controller_;

    // 3. If stream.[[backpressure]] is true,
    if (stream_->had_backpressure_) {
      // a. Let backpressureChangePromise be
      //    stream.[[backpressureChangePromise]].
      StreamPromiseResolver* backpressure_change_promise =
          stream_->backpressure_change_promise_;

      // b. Assert: backpressureChangePromise is not undefined.
      DCHECK(backpressure_change_promise);

      class ResponseFunction final : public PromiseHandlerWithValue {
       public:
        ResponseFunction(ScriptState* script_state,
                         TransformStreamNative* stream,
                         v8::Local<v8::Value> chunk)
            : PromiseHandlerWithValue(script_state),
              stream_(stream),
              chunk_(script_state->GetIsolate(), chunk) {}

        v8::Local<v8::Value> CallWithLocal(v8::Local<v8::Value>) override {
          auto* script_state = GetScriptState();
          auto* isolate = script_state->GetIsolate();

          // c. Return the result of transforming backpressureChangePromise with
          //    a fulfillment handler which performs the following steps:
          //    i. Let writable be stream.[[writable]].
          WritableStreamNative* writable = stream_->writable_;

          //   ii. Let state be writable.[[state]].
          //  iii. If state is "erroring", throw writable.[[storedError]].
          if (writable->IsErroring()) {
            return PromiseReject(script_state,
                                 writable->GetStoredError(isolate));
          }

          // 4. Assert: state is "writable".
          DCHECK(writable->IsWritable());

          // 5. Return ! TransformStreamDefaultControllerPerformTransform(
          //    controller, chunk).
          return TransformStreamDefaultController::PerformTransform(
              script_state, stream_->transform_stream_controller_,
              chunk_.NewLocal(isolate));
        }

        void Trace(Visitor* visitor) override {
          visitor->Trace(stream_);
          visitor->Trace(chunk_);
          ScriptFunction::Trace(visitor);
        }

       private:
        Member<TransformStreamNative> stream_;
        TraceWrapperV8Reference<v8::Value> chunk_;
      };

      // c. Return the result of transforming backpressureChangePromise ...
      return StreamThenPromise(
          script_state->GetContext(),
          backpressure_change_promise->V8Promise(script_state->GetIsolate()),
          MakeGarbageCollected<ResponseFunction>(script_state, stream_, chunk));
    }

    //  4. Return ! TransformStreamDefaultControllerPerformTransform(controller,
    //     chunk).
    return TransformStreamDefaultController::PerformTransform(
        script_state, controller, chunk);
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(stream_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamNative> stream_;
};

class TransformStreamNative::DefaultSinkAbortAlgorithm final
    : public StreamAlgorithm {
 public:
  explicit DefaultSinkAbortAlgorithm(TransformStreamNative* stream)
      : stream_(stream) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 1);
    const auto reason = argv[0];

    // https://streams.spec.whatwg.org/#transform-stream-default-sink-abort-algorithm
    // 1. Perform ! TransformStreamError(stream, reason).
    Error(script_state, stream_, reason);

    // 2. Return a promise resolved with undefined.
    return PromiseResolveWithUndefined(script_state);
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(stream_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamNative> stream_;
};

class TransformStreamNative::DefaultSinkCloseAlgorithm final
    : public StreamAlgorithm {
 public:
  explicit DefaultSinkCloseAlgorithm(TransformStreamNative* stream)
      : stream_(stream) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 0);
    // https://streams.spec.whatwg.org/#transform-stream-default-sink-close-algorithm
    // 1. Let readable be stream.[[readable]].
    ReadableStreamNative* readable = stream_->readable_;

    // 2. Let controller be stream.[[transformStreamController]].
    TransformStreamDefaultController* controller =
        stream_->transform_stream_controller_;

    // 3. Let flushPromise be the result of performing
    //    controller.[[flushAlgorithm]].
    auto flush_promise =
        controller->flush_algorithm_->Run(script_state, 0, nullptr);

    // 4. Perform ! TransformStreamDefaultControllerClearAlgorithms(controller).
    TransformStreamDefaultController::ClearAlgorithms(controller);

    class ResolveFunction final : public PromiseHandlerWithValue {
     public:
      ResolveFunction(ScriptState* script_state, ReadableStreamNative* readable)
          : PromiseHandlerWithValue(script_state), readable_(readable) {}

      v8::Local<v8::Value> CallWithLocal(v8::Local<v8::Value>) override {
        // 5. Return the result of transforming flushPromise with:
        //    a. A fulfillment handler that performs the following steps:
        //       i. If readable.[[state]] is "errored", throw
        //          readable.[[storedError]].
        if (ReadableStreamNative::IsErrored(readable_)) {
          // Returning a rejection is equivalent to throwing here.
          return PromiseReject(
              GetScriptState(),
              readable_->GetStoredError(GetScriptState()->GetIsolate()));
        }

        //      ii. Let readableController be
        //          readable.[[readableStreamController]].
        auto* readable_controller = readable_->GetController();

        //     iii. If ! ReadableStreamDefaultControllerCanCloseOrEnqueue(
        //          readableController) is true, perform !
        //          ReadableStreamDefaultControllerClose(readableController).
        if (ReadableStreamDefaultController::CanCloseOrEnqueue(
                readable_controller)) {
          ReadableStreamDefaultController::Close(GetScriptState(),
                                                 readable_controller);
        }

        return v8::Undefined(GetScriptState()->GetIsolate());
      }

      void Trace(Visitor* visitor) override {
        visitor->Trace(readable_);
        PromiseHandlerWithValue::Trace(visitor);
      }

     private:
      Member<ReadableStreamNative> readable_;
    };

    class RejectFunction final : public PromiseHandlerWithValue {
     public:
      RejectFunction(ScriptState* script_state, TransformStreamNative* stream)
          : PromiseHandlerWithValue(script_state), stream_(stream) {}

      v8::Local<v8::Value> CallWithLocal(v8::Local<v8::Value> r) override {
        // b. A rejection handler that, when called with argument r, performs
        //    the following steps:
        //    i. Perform ! TransformStreamError(stream, r).
        Error(GetScriptState(), stream_, r);

        //   ii. Throw readable.[[storedError]].
        return PromiseReject(
            GetScriptState(),
            stream_->readable_->GetStoredError(GetScriptState()->GetIsolate()));
      }

      void Trace(Visitor* visitor) override {
        visitor->Trace(stream_);
        PromiseHandlerWithValue::Trace(visitor);
      }

     private:
      Member<TransformStreamNative> stream_;
    };

    // 5. Return the result of transforming flushPromise ...
    return StreamThenPromise(
        script_state->GetContext(), flush_promise,
        MakeGarbageCollected<ResolveFunction>(script_state, readable),
        MakeGarbageCollected<RejectFunction>(script_state, stream_));
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(stream_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamNative> stream_;
};

class TransformStreamNative::DefaultSourcePullAlgorithm final
    : public StreamAlgorithm {
 public:
  explicit DefaultSourcePullAlgorithm(TransformStreamNative* stream)
      : stream_(stream) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 0);

    // https://streams.spec.whatwg.org/#transform-stream-default-source-pull
    // 1. Assert: stream.[[backpressure]] is true.
    DCHECK(stream_->had_backpressure_);

    // 2. Assert: stream.[[backpressureChangePromise]] is not undefined.
    DCHECK(stream_->backpressure_change_promise_);

    // 3. Perform ! TransformStreamSetBackpressure(stream, false).
    SetBackpressure(script_state, stream_, false);

    // 4. Return stream.[[backpressureChangePromise]].
    return stream_->backpressure_change_promise_->V8Promise(
        script_state->GetIsolate());
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(stream_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamNative> stream_;
};

// This algorithm isn't explicitly named in the standard, but instead is
// described by a step in InitializeTransformStream.
class TransformStreamNative::DefaultSourceCancelAlgorithm final
    : public StreamAlgorithm {
 public:
  explicit DefaultSourceCancelAlgorithm(TransformStreamNative* stream)
      : stream_(stream) {}

  v8::Local<v8::Promise> Run(ScriptState* script_state,
                             int argc,
                             v8::Local<v8::Value> argv[]) override {
    DCHECK_EQ(argc, 1);

    // https://streams.spec.whatwg.org/#initialize-transform-stream
    // 7. Let cancelAlgorithm be the following steps, taking a reason argument:
    //    a. Perform ! TransformStreamErrorWritableAndUnblockWrite(stream,
    //       reason).
    ErrorWritableAndUnblockWrite(script_state, stream_, argv[0]);

    //    b. Return a promise resolved with undefined.
    return PromiseResolveWithUndefined(script_state);
  }

  void Trace(Visitor* visitor) override {
    visitor->Trace(stream_);
    StreamAlgorithm::Trace(visitor);
  }

 private:
  Member<TransformStreamNative> stream_;
};

void TransformStreamNative::Initialize(
    ScriptState* script_state,
    TransformStreamNative* stream,
    StreamPromiseResolver* start_promise,
    double writable_high_water_mark,
    StrategySizeAlgorithm* writable_size_algorithm,
    double readable_high_water_mark,
    StrategySizeAlgorithm* readable_size_algorithm) {
  ExceptionState exception_state(script_state->GetIsolate(),
                                 ExceptionState::kUnknownContext, "", "");

  // https://streams.spec.whatwg.org/#initialize-transform-stream
  // 1. Let startAlgorithm be an algorithm that returns startPromise.
  auto* start_algorithm =
      MakeGarbageCollected<ReturnStartPromiseAlgorithm>(start_promise);

  // 2. Let writeAlgorithm be the following steps, taking a chunk argument:
  //    a. Return ! TransformStreamDefaultSinkWriteAlgorithm(stream, chunk).
  auto* write_algorithm =
      MakeGarbageCollected<DefaultSinkWriteAlgorithm>(stream);

  // 3. Let abortAlgorithm be the following steps, taking a reason argument:
  //    a. Return ! TransformStreamDefaultSinkAbortAlgorithm(stream, reason).
  auto* abort_algorithm =
      MakeGarbageCollected<DefaultSinkAbortAlgorithm>(stream);

  // 4. Let closeAlgorithm be the following steps:
  //    a. Return ! TransformStreamDefaultSinkCloseAlgorithm(stream).
  auto* close_algorithm =
      MakeGarbageCollected<DefaultSinkCloseAlgorithm>(stream);

  // 5. Set stream.[[writable]] to ! CreateWritableStream(startAlgorithm,
  //    writeAlgorithm, closeAlgorithm, abortAlgorithm, writableHighWaterMark,
  //    writableSizeAlgorithm).
  stream->writable_ = WritableStreamNative::Create(
      script_state, start_algorithm, write_algorithm, close_algorithm,
      abort_algorithm, writable_high_water_mark, writable_size_algorithm,
      exception_state);
  DCHECK(!exception_state.HadException());

  // 6. Let pullAlgorithm be the following steps:
  //    a. Return ! TransformStreamDefaultSourcePullAlgorithm(stream).
  auto* pull_algorithm =
      MakeGarbageCollected<DefaultSourcePullAlgorithm>(stream);

  // 7. Let cancelAlgorithm be the following steps, taking a reason argument:
  //    a. Perform ! TransformStreamErrorWritableAndUnblockWrite(stream,
  //       reason).
  //    b. Return a promise resolved with undefined.
  auto* cancel_algorithm =
      MakeGarbageCollected<DefaultSourceCancelAlgorithm>(stream);

  // 8. Set stream.[[readable]] to ! CreateReadableStream(startAlgorithm,
  //    pullAlgorithm, cancelAlgorithm, readableHighWaterMark,
  //    readableSizeAlgorithm).
  stream->readable_ = ReadableStreamNative::Create(
      script_state, start_algorithm, pull_algorithm, cancel_algorithm,
      readable_high_water_mark, readable_size_algorithm, exception_state);
  DCHECK(!exception_state.HadException());

  //  9. Set stream.[[backpressure]] and stream.[[backpressureChangePromise]] to
  //     undefined.
  // 10. Perform ! TransformStreamSetBackpressure(stream, true).
  // |had_backpressure_| is bool and so can't be set to undefined; instead we
  // take the equivalent steps to achieve the final result here.
  DCHECK(stream->had_backpressure_);
  DCHECK(!stream->backpressure_change_promise_);
  stream->backpressure_change_promise_ =
      MakeGarbageCollected<StreamPromiseResolver>(script_state);

  // 11. Set stream.[[transformStreamController]] to undefined.
  // (This is set by the constructor; just verify the value here).
  DCHECK_EQ(stream->transform_stream_controller_, nullptr);
}

void TransformStreamNative::Error(ScriptState* script_state,
                                  TransformStreamNative* stream,
                                  v8::Local<v8::Value> e) {
  // https://streams.spec.whatwg.org/#transform-stream-error
  // 1. Perform ! ReadableStreamDefaultControllerError(stream.[[readable]].
  //    [[readableStreamController]], e).
  ReadableStreamNative* readable = stream->readable_;
  ReadableStreamDefaultController::Error(script_state,
                                         readable->GetController(), e);

  // 2. Perform ! TransformStreamErrorWritableAndUnblockWrite(stream, e).
  ErrorWritableAndUnblockWrite(script_state, stream, e);
}

void TransformStreamNative::ErrorWritableAndUnblockWrite(
    ScriptState* script_state,
    TransformStreamNative* stream,
    v8::Local<v8::Value> e) {
  // https://streams.spec.whatwg.org/#transform-stream-error-writable-and-unblock-write
  // 1. Perform ! TransformStreamDefaultControllerClearAlgorithms(stream.
  //    [[transformStreamController]]).
  TransformStreamDefaultController::ClearAlgorithms(
      stream->transform_stream_controller_);

  // 2. Perform !
  //    WritableStreamDefaultControllerErrorIfNeeded(stream.[[writable]].
  //    [[writableStreamController]], e).
  WritableStreamDefaultController::ErrorIfNeeded(
      script_state, stream->writable_->Controller(), e);

  // 3. If stream.[[backpressure]] is true, perform !
  //    TransformStreamSetBackpressure(stream, false).
  if (stream->had_backpressure_) {
    SetBackpressure(script_state, stream, false);
  }
}

void TransformStreamNative::SetBackpressure(ScriptState* script_state,
                                            TransformStreamNative* stream,
                                            bool backpressure) {
  // https://streams.spec.whatwg.org/#transform-stream-set-backpressure
  // 1. Assert: stream.[[backpressure]] is not backpressure.
  DCHECK(stream->had_backpressure_ != backpressure);

  // 2. If stream.[[backpressureChangePromise]] is not undefined, resolve
  //    stream.[[backpressureChangePromise]] with undefined.
  // In the standard, [[backpressureChangePromise]] is initialized by calling
  // this function. However, in this implementation it is initialized in
  // InitializeTransformStream() without calling this function. As a result,
  // the function is never called without |backpressure_change_promise_| set
  // and we don't need to test it.
  DCHECK(stream->backpressure_change_promise_);
  stream->backpressure_change_promise_->ResolveWithUndefined(script_state);

  // 3. Set stream.[[backpressureChangePromise]] to a new promise.
  stream->backpressure_change_promise_ =
      MakeGarbageCollected<StreamPromiseResolver>(script_state);

  // 4. Set stream.[[backpressure]] to backpressure.
  stream->had_backpressure_ = backpressure;
}

}  // namespace blink
