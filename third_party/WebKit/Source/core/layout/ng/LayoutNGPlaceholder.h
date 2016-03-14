// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NGPlaceholder_h
#define NGPlaceholder_h

#include "core/layout/LayoutObject.h"
#include "core/layout/ng/NGBox.h"
#include "platform/geometry/LayoutRect.h"
#include "wtf/OwnPtr.h"

namespace blink {

// This is a transitional placeholder in the LayoutObject tree. Its members should
// eventually live in ComputedStyle, and it mostly serves to delegate to its NGBoxen.
class LayoutNGPlaceholder : public LayoutBoxModelObject {
    explicit LayoutNGPlaceholder(ContainerNode*);

    LayoutNGPlaceholder* rootLayoutNGPlaceholder() const;
private:
    OwnPtr<Vector<NGBox*>> m_boxes;
    OwnPtr<NGBox> m_box;
};

} // namespace blink

#endif // NGPlaceholder_h
