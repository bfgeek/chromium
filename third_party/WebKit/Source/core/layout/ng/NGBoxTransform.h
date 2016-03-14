// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NGBoxTransform_h
#define NGBoxTransform_h

namespace blink {

class NGBox;

class NGBoxTransform {

private:
    LayoutPoint m_position;
    NGBox& m_box;
    // TODO(leviw): Add arbitrary transform here

    NGBoxTransform* m_next;
    NGBoxTransform* m_previous;
    NGBoxTransform* m_parent;
    NGBoxTransform* m_firstChild;
    NGBoxTransform* m_lastChild;
};

} // namespace blink

#endif // NGBoxTransform_h
