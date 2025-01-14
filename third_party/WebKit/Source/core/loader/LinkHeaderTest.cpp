// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/loader/LinkHeader.h"

#include "testing/gtest/include/gtest/gtest.h"
#include <base/macros.h>

namespace blink {
namespace {

TEST(LinkHeaderTest, Empty)
{
    String nullString;
    LinkHeaderSet nullHeaderSet(nullString);
    ASSERT_EQ(nullHeaderSet.size(), unsigned(0));
    String emptyString("");
    LinkHeaderSet emptyHeaderSet(emptyString);
    ASSERT_EQ(emptyHeaderSet.size(), unsigned(0));
}

struct SingleTestCase {
    const char* headerValue;
    const char* url;
    const char* rel;
    const char* as;
    bool valid;
} singleTestCases[] = {
    {"</images/cat.jpg>; rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>;rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>   ;rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>   ;   rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"< /images/cat.jpg>   ;   rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg >   ;   rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg wutwut>   ;   rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg wutwut  \t >   ;   rel=prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; rel=prefetch   ", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; Rel=prefetch   ", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; Rel=PReFetCh   ", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; rel=prefetch; rel=somethingelse", "/images/cat.jpg", "prefetch", "", true},
    {"  </images/cat.jpg>; rel=prefetch   ", "/images/cat.jpg", "prefetch", "", true},
    {"\t  </images/cat.jpg>; rel=prefetch   ", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>\t\t ; \trel=prefetch \t  ", "/images/cat.jpg", "prefetch", "", true},
    {"\f</images/cat.jpg>\t\t ; \trel=prefetch \t  ", "", "", "", false},
    {"</images/cat.jpg>; rel= prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"<../images/cat.jpg?dog>; rel= prefetch", "../images/cat.jpg?dog", "prefetch", "", true},
    {"</images/cat.jpg>; rel =prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; rel pel=prefetch", "/images/cat.jpg", "", "", false},
    {"< /images/cat.jpg>", "/images/cat.jpg", "", "", true},
    {"</images/cat.jpg>; rel =", "/images/cat.jpg", "", "", false},
    {"</images/cat.jpg>; wut=sup; rel =prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; wut=sup ; rel =prefetch", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; wut=sup ; rel =prefetch  \t  ;", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg> wut=sup ; rel =prefetch  \t  ;", "/images/cat.jpg", "", "", false},
    {"<   /images/cat.jpg", "", "", "", false},
    {"<   http://wut.com/  sdfsdf ?sd>; rel=dns-prefetch", "http://wut.com/", "dns-prefetch", "", true},
    {"<   http://wut.com/%20%20%3dsdfsdf?sd>; rel=dns-prefetch", "http://wut.com/%20%20%3dsdfsdf?sd", "dns-prefetch", "", true},
    {"<   http://wut.com/dfsdf?sdf=ghj&wer=rty>; rel=prefetch", "http://wut.com/dfsdf?sdf=ghj&wer=rty", "prefetch", "", true},
    {"<   http://wut.com/dfsdf?sdf=ghj&wer=rty>;;;;; rel=prefetch", "http://wut.com/dfsdf?sdf=ghj&wer=rty", "prefetch", "", true},
    {"<   http://wut.com/%20%20%3dsdfsdf?sd>; rel=preload;as=image", "http://wut.com/%20%20%3dsdfsdf?sd", "preload", "image", true},
    {"<   http://wut.com/%20%20%3dsdfsdf?sd>; rel=preload;as=whatever", "http://wut.com/%20%20%3dsdfsdf?sd", "preload", "whatever", true},
    {"</images/cat.jpg>; anchor=foo; rel=prefetch;", "/images/cat.jpg", "", "", false},
    {"</images/cat.jpg>; rel=prefetch;anchor=foo ", "/images/cat.jpg", "prefetch", "", false},
    {"</images/cat.jpg>; anchor='foo'; rel=prefetch;", "/images/cat.jpg", "", "", false},
    {"</images/cat.jpg>; rel=prefetch;anchor='foo' ", "/images/cat.jpg", "prefetch", "", false},
    {"</images/cat.jpg>; rel=prefetch;anchor='' ", "/images/cat.jpg", "prefetch", "", false},
    {"</images/cat.jpg>; rel=prefetch;", "/images/cat.jpg", "prefetch", "", true},
    {"</images/cat.jpg>; rel=prefetch    ;", "/images/cat.jpg", "prefetch", "", true},
    {"</images/ca,t.jpg>; rel=prefetch    ;", "/images/ca,t.jpg", "prefetch", "", true},
    {"<simple.css>; rel=stylesheet; title=\"title with a DQUOTE and backslash\"", "simple.css", "stylesheet", "", true},
    {"<simple.css>; rel=stylesheet; title=\"title with a DQUOTE \\\" and backslash: \\\"", "simple.css", "stylesheet", "", false},
    {"<simple.css>; title=\"title with a DQUOTE \\\" and backslash: \"; rel=stylesheet; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; title=\'title with a DQUOTE \\\' and backslash: \'; rel=stylesheet; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; title=\"title with a DQUOTE \\\" and ;backslash,: \"; rel=stylesheet; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; title=\"title with a DQUOTE \' and ;backslash,: \"; rel=stylesheet; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; title=\"\"; rel=stylesheet; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; title=\"\"; rel=\"stylesheet\"; ", "simple.css", "stylesheet", "", true},
    {"<simple.css>; rel=stylesheet; title=\"", "simple.css", "stylesheet", "", false},
    {"<simple.css>; rel=stylesheet; title=\"\"", "simple.css", "stylesheet", "", true},
    {"<simple.css>; rel=\"stylesheet\"; title=\"", "simple.css", "stylesheet", "", false},
    {"<simple.css>; rel=\";style,sheet\"; title=\"", "simple.css", ";style,sheet", "", false},
    {"<simple.css>; rel=\"bla'sdf\"; title=\"", "simple.css", "bla'sdf", "", false},
    {"<simple.css>; rel=\"\"; title=\"\"", "simple.css", "", "", true},
    {"<simple.css>; rel=''; title=\"\"", "simple.css", "", "", true},
    {"<simple.css>; rel=''; title=", "simple.css", "", "", false},
    {"<simple.css>; rel=''; title", "simple.css", "", "", false},
    {"<simple.css>; rel=''; media", "simple.css", "", "", false},
    {"<simple.css>; rel=''; hreflang", "simple.css", "", "", false},
    {"<simple.css>; rel=''; type", "simple.css", "", "", false},
    {"<simple.css>; rel=''; rev", "simple.css", "", "", false},
    {"<simple.css>; rel=''; bla", "simple.css", "", "", true},
    {"<simple.css>; rel='prefetch", "simple.css", "", "", false},
    {"<simple.css>; rel=\"prefetch", "simple.css", "", "", false},
    {"<simple.css>; rel=\"", "simple.css", "", "", false},
    {"<http://whatever.com>; rel=preconnect; valid!", "http://whatever.com", "preconnect", "", true},
    {"<http://whatever.com>; rel=preconnect; valid$", "http://whatever.com", "preconnect", "", true},
    {"<http://whatever.com>; rel=preconnect; invalid@", "http://whatever.com", "preconnect", "", false},
    {"<http://whatever.com>; rel=preconnect; invalid*", "http://whatever.com", "preconnect", "", false},
};

void PrintTo(const SingleTestCase& test, std::ostream* os)
{
    *os << ::testing::PrintToString(test.headerValue);
}

class SingleLinkHeaderTest : public ::testing::TestWithParam<SingleTestCase> {};

// Test the cases with a single header
TEST_P(SingleLinkHeaderTest, Single)
{
    const SingleTestCase testCase = GetParam();
    LinkHeaderSet headerSet(testCase.headerValue);
    ASSERT_EQ(1u, headerSet.size());
    LinkHeader& header = headerSet[0];
    EXPECT_STREQ(testCase.url, header.url().ascii().data());
    EXPECT_STREQ(testCase.rel, header.rel().ascii().data());
    EXPECT_EQ(testCase.valid, header.valid());
}

INSTANTIATE_TEST_CASE_P(LinkHeaderTest, SingleLinkHeaderTest, testing::ValuesIn(singleTestCases));

struct DoubleTestCase {
    const char* headerValue;
    const char* url;
    const char* rel;
    bool valid;
    const char* url2;
    const char* rel2;
    bool valid2;
} doubleTestCases[] = {
    {"<ybg.css>; rel=stylesheet, <simple.css>; rel=stylesheet", "ybg.css", "stylesheet", true, "simple.css", "stylesheet", true},
    {"<ybg.css>; rel=stylesheet,<simple.css>; rel=stylesheet", "ybg.css", "stylesheet", true, "simple.css", "stylesheet", true},
    {"<ybg.css>; rel=stylesheet;crossorigin,<simple.css>; rel=stylesheet", "ybg.css", "stylesheet", true, "simple.css", "stylesheet", true},
};

void PrintTo(const DoubleTestCase& test, std::ostream* os)
{
    *os << ::testing::PrintToString(test.headerValue);
}

class DoubleLinkHeaderTest : public ::testing::TestWithParam<DoubleTestCase> {};

TEST_P(DoubleLinkHeaderTest, Double)
{
    const DoubleTestCase testCase = GetParam();
    LinkHeaderSet headerSet(testCase.headerValue);
    ASSERT_EQ(2u, headerSet.size());
    LinkHeader& header1 = headerSet[0];
    LinkHeader& header2 = headerSet[1];
    EXPECT_STREQ(testCase.url, header1.url().ascii().data());
    EXPECT_STREQ(testCase.rel, header1.rel().ascii().data());
    EXPECT_EQ(testCase.valid, header1.valid());
    EXPECT_STREQ(testCase.url2, header2.url().ascii().data());
    EXPECT_STREQ(testCase.rel2, header2.rel().ascii().data());
    EXPECT_EQ(testCase.valid2, header2.valid());
}

INSTANTIATE_TEST_CASE_P(LinkHeaderTest, DoubleLinkHeaderTest, testing::ValuesIn(doubleTestCases));

struct CrossOriginTestCase {
    const char* headerValue;
    const char* url;
    const char* rel;
    const CrossOriginAttributeValue crossorigin;
    bool valid;
} crossOriginTestCases[] = {
    {"<http://whatever.com>; rel=preconnect", "http://whatever.com", "preconnect", CrossOriginAttributeNotSet, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin=", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin ", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin;", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin, <http://whatever2.com>; rel=preconnect", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin , <http://whatever2.com>; rel=preconnect", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin,<http://whatever2.com>; rel=preconnect", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin=anonymous", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin=use-credentials", "http://whatever.com", "preconnect", CrossOriginAttributeUseCredentials, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin=whatever", "http://whatever.com", "preconnect", CrossOriginAttributeAnonymous, true},
    {"<http://whatever.com>; rel=preconnect; crossorig|in=whatever", "http://whatever.com", "preconnect", CrossOriginAttributeNotSet, true},
    {"<http://whatever.com>; rel=preconnect; crossorigin|=whatever", "http://whatever.com", "preconnect", CrossOriginAttributeNotSet, true},
};

void PrintTo(const CrossOriginTestCase& test, std::ostream* os)
{
    *os << ::testing::PrintToString(test.headerValue);
}

class CrossOriginLinkHeaderTest : public ::testing::TestWithParam<CrossOriginTestCase> {};

TEST_P(CrossOriginLinkHeaderTest, CrossOrigin)
{
    const CrossOriginTestCase testCase = GetParam();
    LinkHeaderSet headerSet(testCase.headerValue);
    ASSERT_GE(headerSet.size(), 1u);
    LinkHeader& header = headerSet[0];
    EXPECT_STREQ(testCase.url, header.url().ascii().data());
    EXPECT_STREQ(testCase.rel, header.rel().ascii().data());
    EXPECT_EQ(testCase.crossorigin, header.crossOrigin());
    EXPECT_EQ(testCase.valid, header.valid());
}

INSTANTIATE_TEST_CASE_P(LinkHeaderTest, CrossOriginLinkHeaderTest, testing::ValuesIn(crossOriginTestCases));

} // namespace
} // namespace blink
