// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LayoutBuilderQueue_h
#define LayoutBuilderQueue_h

#include "wtf/Deque.h"

namespace blink {

class NGBlockBuilder;

class LayoutBuilderQueue {

private:
    WTF::Deque<NGBlockBuilder*> m_queue; // TODO(leviw): This should be NGBuilders (a base class).
};

} // namespace blink

#endif // LayoutBuilderQueue_h
