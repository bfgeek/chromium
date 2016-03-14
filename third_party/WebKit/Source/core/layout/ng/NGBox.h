// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NGBox_h
#define NGBox_h

#include "core/layout/ng/NGBoxTransform.h"
#include "core/style/ComputedStyle.h"
#include "platform/geometry/LayoutRect.h"

namespace blink {

class NGBox {

private:
    NGBoxTransform* m_transform; // This should be the only mutable part of the tree.

    const LayoutSize m_size;

    // This should eventually point to ComputedStyle/the NGBox collection instread
    // of a placeholder.
    LayoutNGPlaceholder* m_placeholder;
};

} // namespace blink

#endif // NGBox_h
