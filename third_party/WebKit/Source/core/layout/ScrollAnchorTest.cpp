// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/layout/ScrollAnchor.h"

#include "core/layout/LayoutBox.h"
#include "core/layout/LayoutTestHelper.h"
#include "core/paint/PaintLayerScrollableArea.h"

namespace blink {

using Corner = ScrollAnchor::Corner;

class ScrollAnchorTest : public RenderingTest {
public:
    ScrollAnchorTest() { RuntimeEnabledFeatures::setScrollAnchoringEnabled(true); }
    ~ScrollAnchorTest() { RuntimeEnabledFeatures::setScrollAnchoringEnabled(false); }

protected:
    void update()
    {
        // TODO(skobes): Use SimTest instead of RenderingTest and move into Source/web?
        document().view()->updateAllLifecyclePhases();
    }

    ScrollableArea* layoutViewport()
    {
        return document().view()->layoutViewportScrollableArea();
    }

    ScrollableArea* scrollerForElement(Element* element)
    {
        return toLayoutBox(element->layoutObject())->getScrollableArea();
    }

    ScrollAnchor& scrollAnchor(ScrollableArea* scroller)
    {
        if (scroller->isFrameView())
            return toFrameView(scroller)->scrollAnchor();
        ASSERT(scroller->isPaintLayerScrollableArea());
        return toPaintLayerScrollableArea(scroller)->scrollAnchor();
    }

    void setHeight(Element* element, int height)
    {
        element->setAttribute(HTMLNames::styleAttr,
            AtomicString(String::format("height: %dpx", height)));
        update();
    }

    void scrollLayoutViewport(DoubleSize delta)
    {
        Element* scrollingElement = document().scrollingElement();
        if (delta.width())
            scrollingElement->setScrollTop(scrollingElement->scrollLeft() + delta.width());
        if (delta.height())
            scrollingElement->setScrollTop(scrollingElement->scrollTop() + delta.height());
    }
};

TEST_F(ScrollAnchorTest, Basic)
{
    setBodyInnerHTML(
        "<style> body { height: 1000px } div { height: 100px } </style>"
        "<div id='block1'>abc</div>"
        "<div id='block2'>def</div>");

    ScrollableArea* viewport = layoutViewport();

    // No anchor at origin (0,0).
    EXPECT_EQ(nullptr, scrollAnchor(viewport).anchorObject());

    scrollLayoutViewport(DoubleSize(0, 150));
    setHeight(document().getElementById("block1"), 200);

    EXPECT_EQ(250, viewport->scrollPosition().y());
    EXPECT_EQ(document().getElementById("block2")->layoutObject(),
        scrollAnchor(viewport).anchorObject());

    // ScrollableArea::userScroll should clear the anchor.
    viewport->userScroll(ScrollByPrecisePixel, FloatSize(0, 100));
    EXPECT_EQ(nullptr, scrollAnchor(viewport).anchorObject());
}

TEST_F(ScrollAnchorTest, AnchorWithLayerInScrollingDiv)
{
    setBodyInnerHTML(
        "<style>"
        "    #scroller { overflow: scroll; width: 500px; height: 400px; }"
        "    div { height: 100px }"
        "    #block2 { overflow: hidden }"
        "    #space { height: 1000px; }"
        "</style>"
        "<div id='scroller'><div id='space'>"
        "<div id='block1'>abc</div>"
        "<div id='block2'>def</div>"
        "</div></div>");

    ScrollableArea* scroller = scrollerForElement(document().getElementById("scroller"));
    Element* block1 = document().getElementById("block1");
    Element* block2 = document().getElementById("block2");

    scroller->scrollBy(DoubleSize(0, 150), UserScroll);

    // In this layout pass we will anchor to #block2 which has its own PaintLayer.
    setHeight(block1, 200);
    EXPECT_EQ(250, scroller->scrollPosition().y());
    EXPECT_EQ(block2->layoutObject(), scrollAnchor(scroller).anchorObject());

    // Test that the anchor object can be destroyed without affecting the scroll position.
    block2->remove();
    update();
    EXPECT_EQ(250, scroller->scrollPosition().y());
}

TEST_F(ScrollAnchorTest, FullyContainedInlineBlock)
{
    // Exercises every WalkStatus value:
    // html, body -> Constrain
    // #outer -> Continue
    // #ib1, br -> Skip
    // #ib2 -> Return
    setBodyInnerHTML(
        "<style>"
        "    body { height: 1000px }"
        "    #outer { line-height: 100px }"
        "    #ib1, #ib2 { display: inline-block }"
        "</style>"
        "<span id=outer>"
        "    <span id=ib1>abc</span>"
        "    <br><br>"
        "    <span id=ib2>def</span>"
        "</span>");

    scrollLayoutViewport(DoubleSize(0, 150));

    Element* ib2 = document().getElementById("ib2");
    ib2->setAttribute(HTMLNames::styleAttr, "line-height: 150px");
    update();
    EXPECT_EQ(ib2->layoutObject(), scrollAnchor(layoutViewport()).anchorObject());
}

TEST_F(ScrollAnchorTest, TextBounds)
{
    setBodyInnerHTML(
        "<style>"
        "    body {"
        "        position: absolute;"
        "        font-size: 100px;"
        "        width: 200px;"
        "        height: 1000px;"
        "        line-height: 100px;"
        "    }"
        "</style>"
        "abc <b id=b>def</b> ghi");

    scrollLayoutViewport(DoubleSize(0, 150));

    setHeight(document().body(), 1100);
    EXPECT_EQ(document().getElementById("b")->layoutObject()->slowFirstChild(),
        scrollAnchor(layoutViewport()).anchorObject());
}

TEST_F(ScrollAnchorTest, ExcludeFixedPosition)
{
    setBodyInnerHTML(
        "<style>"
        "    body { height: 1000px; padding: 20px }"
        "    div { position: relative; top: 100px; }"
        "    #f { position: fixed }"
        "</style>"
        "<div id=f>fixed</div>"
        "<div id=c>content</div>");

    scrollLayoutViewport(DoubleSize(0, 50));

    setHeight(document().body(), 1100);
    EXPECT_EQ(document().getElementById("c")->layoutObject(),
        scrollAnchor(layoutViewport()).anchorObject());
}

class ScrollAnchorCornerTest : public ScrollAnchorTest {
protected:
    void checkCorner(const AtomicString& id, Corner corner, DoublePoint startPos, DoubleSize expectedAdjustment)
    {
        ScrollableArea* viewport = layoutViewport();
        Element* element = document().getElementById(id);

        viewport->setScrollPosition(startPos, UserScroll);
        element->setAttribute(HTMLNames::classAttr, "big");
        update();

        DoublePoint endPos = startPos;
        endPos.move(expectedAdjustment);

        EXPECT_EQ(endPos, viewport->scrollPositionDouble());
        EXPECT_EQ(element->layoutObject(), scrollAnchor(viewport).anchorObject());
        EXPECT_EQ(corner, scrollAnchor(viewport).corner());

        element->removeAttribute(HTMLNames::classAttr);
        update();
    }
};

TEST_F(ScrollAnchorCornerTest, Corners)
{
    setBodyInnerHTML(
        "<style>"
        "    body {"
        "        position: absolute; border: 10px solid #ccc;"
        "        width: 1220px; height: 920px;"
        "    }"
        "    #a, #b, #c, #d {"
        "        position: absolute; background-color: #ace;"
        "        width: 400px; height: 300px;"
        "    }"
        "    #a, #b { top: 0; }"
        "    #a, #c { left: 0; }"
        "    #b, #d { right: 0; }"
        "    #c, #d { bottom: 0; }"
        "    .big { width: 800px !important; height: 600px !important }"
        "</style>"
        "<div id=a></div>"
        "<div id=b></div>"
        "<div id=c></div>"
        "<div id=d></div>");

    checkCorner("a", Corner::BottomRight, DoublePoint(20,  20),  DoubleSize(+400, +300));
    checkCorner("b", Corner::BottomLeft,  DoublePoint(420, 20),  DoubleSize(-400, +300));
    checkCorner("c", Corner::TopRight,    DoublePoint(20,  320), DoubleSize(+400, -300));
    checkCorner("d", Corner::TopLeft,     DoublePoint(420, 320), DoubleSize(-400, -300));
}

}
