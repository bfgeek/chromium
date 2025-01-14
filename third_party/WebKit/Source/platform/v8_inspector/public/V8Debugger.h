// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8Debugger_h
#define V8Debugger_h

#include "platform/PlatformExport.h"
#include "platform/inspector_protocol/Frontend.h"
#include "wtf/PassOwnPtr.h"

#include <v8.h>

namespace blink {

class V8ContextInfo;
class V8DebuggerClient;
class V8StackTrace;

namespace protocol {
class DictionaryValue;
}

class PLATFORM_EXPORT V8Debugger {
public:
    template <typename T>
    class Agent {
    public:
        virtual void setInspectorState(protocol::DictionaryValue*) = 0;
        virtual void setFrontend(T*) = 0;
        virtual void clearFrontend() = 0;
        virtual void restore() = 0;
    };

    static PassOwnPtr<V8Debugger> create(v8::Isolate*, V8DebuggerClient*);
    virtual ~V8Debugger() { }

    // Each v8::Context is a part of a group. The group id is used to find approapriate
    // V8DebuggerAgent to notify about events in the context.
    // |contextGroupId| must be non-0.
    static void setContextDebugData(v8::Local<v8::Context>, const String16& type, int contextGroupId);
    static int contextId(v8::Local<v8::Context>);

    // Context should have been already marked with |setContextDebugData| call.
    virtual void contextCreated(const V8ContextInfo&) = 0;
    virtual void contextDestroyed(v8::Local<v8::Context>) = 0;

    static v8::Local<v8::Symbol> commandLineAPISymbol(v8::Isolate*);
    static bool isCommandLineAPIMethod(const String16& name);

    virtual PassOwnPtr<V8StackTrace> createStackTrace(v8::Local<v8::StackTrace>, size_t maxStackSize) = 0;
    virtual PassOwnPtr<V8StackTrace> captureStackTrace(size_t maxStackSize) = 0;
};

} // namespace blink


#endif // V8Debugger_h
