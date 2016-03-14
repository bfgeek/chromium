// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NGBlockBuilder_h
#define NGBlockBuilder_h

#include "core/layout/ng/NGBox.h"
#include "core/layout/ng/NGBoxTransform.h"
#include "core/style/ComputedStyle.h"
#include "platform/geometry/LayoutRect.h"

namespace blink {

class LayoutContext;

class NGBlockBuilder {
    NGBlockBuilder(LayoutContext*); // Also does Layout

private:
    NGBoxTransform* m_transform; // This should be the only mutable piece

    LayoutSize m_size;
    ComputedStyle& m_style; // Can also give us access to other fragments.
};

} // namespace blink

#endif // NGBlockBuilder_h
