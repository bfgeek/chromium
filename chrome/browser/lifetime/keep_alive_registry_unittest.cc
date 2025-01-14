// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/lifetime/keep_alive_registry.h"

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/lifetime/keep_alive_state_observer.h"
#include "chrome/browser/lifetime/keep_alive_types.h"
#include "chrome/browser/lifetime/scoped_keep_alive.h"
#include "chrome/test/base/testing_browser_process.h"
#include "testing/gtest/include/gtest/gtest.h"

class KeepAliveRegistryTest : public testing::Test,
                              public KeepAliveStateObserver {
 public:
  KeepAliveRegistryTest()
      : on_restart_allowed_call_count_(0),
        on_restart_forbidden_call_count_(0),
        registry_(KeepAliveRegistry::GetInstance()) {
    registry_->AddObserver(this);

    EXPECT_FALSE(registry_->IsKeepingAlive());
  }

  ~KeepAliveRegistryTest() override {
    registry_->RemoveObserver(this);

    EXPECT_FALSE(registry_->IsKeepingAlive());
  }

  void OnKeepAliveRestartStateChanged(bool can_restart) override {
    if (can_restart)
      ++on_restart_allowed_call_count_;
    else
      ++on_restart_forbidden_call_count_;
  }

 protected:
  int on_restart_allowed_call_count_;
  int on_restart_forbidden_call_count_;
  KeepAliveRegistry* registry_;
};

// Test the IsKeepingAlive state and when we interact with the browser with
// a KeepAlive registered.
TEST_F(KeepAliveRegistryTest, BasicKeepAliveTest) {
  TestingBrowserProcess* browser_process = TestingBrowserProcess::GetGlobal();
  const unsigned int base_module_ref_count =
      browser_process->module_ref_count();
  KeepAliveRegistry* registry = KeepAliveRegistry::GetInstance();

  EXPECT_FALSE(registry->IsKeepingAlive());

  {
    // Arbitrarily chosen Origin
    ScopedKeepAlive test_keep_alive(KeepAliveOrigin::CHROME_APP_DELEGATE,
                                    KeepAliveRestartOption::DISABLED);

    // We should require the browser to stay alive
    EXPECT_EQ(base_module_ref_count + 1, browser_process->module_ref_count());
    EXPECT_TRUE(registry_->IsKeepingAlive());
  }

  // We should be back to normal now.
  EXPECT_EQ(base_module_ref_count, browser_process->module_ref_count());
  EXPECT_FALSE(registry_->IsKeepingAlive());
}

// Test the IsKeepingAlive state and when we interact with the browser with
// more than one KeepAlive registered.
TEST_F(KeepAliveRegistryTest, DoubleKeepAliveTest) {
  TestingBrowserProcess* browser_process = TestingBrowserProcess::GetGlobal();
  const unsigned int base_module_ref_count =
      browser_process->module_ref_count();
  scoped_ptr<ScopedKeepAlive> keep_alive_1, keep_alive_2;

  keep_alive_1.reset(new ScopedKeepAlive(KeepAliveOrigin::CHROME_APP_DELEGATE,
                                         KeepAliveRestartOption::DISABLED));
  EXPECT_EQ(base_module_ref_count + 1, browser_process->module_ref_count());
  EXPECT_TRUE(registry_->IsKeepingAlive());

  keep_alive_2.reset(new ScopedKeepAlive(KeepAliveOrigin::CHROME_APP_DELEGATE,
                                         KeepAliveRestartOption::DISABLED));
  // We should not increment the count twice
  EXPECT_EQ(base_module_ref_count + 1, browser_process->module_ref_count());
  EXPECT_TRUE(registry_->IsKeepingAlive());

  keep_alive_1.reset();
  // We should not decrement the count before the last keep alive is released.
  EXPECT_EQ(base_module_ref_count + 1, browser_process->module_ref_count());
  EXPECT_TRUE(registry_->IsKeepingAlive());

  keep_alive_2.reset();
  EXPECT_EQ(base_module_ref_count, browser_process->module_ref_count());
  EXPECT_FALSE(registry_->IsKeepingAlive());
}

// Test the IsKeepingAlive state and when we interact with the browser with
// more than one KeepAlive registered.
TEST_F(KeepAliveRegistryTest, RestartOptionTest) {
  scoped_ptr<ScopedKeepAlive> keep_alive, keep_alive_restart;

  EXPECT_EQ(0, on_restart_allowed_call_count_);
  EXPECT_EQ(0, on_restart_forbidden_call_count_);

  // With a normal keep alive, restart should not be allowed
  keep_alive.reset(new ScopedKeepAlive(KeepAliveOrigin::CHROME_APP_DELEGATE,
                                       KeepAliveRestartOption::DISABLED));
  ASSERT_EQ(1, on_restart_forbidden_call_count_--);  // decrement to ack

  // Restart should not be allowed if all KA don't allow it.
  keep_alive_restart.reset(new ScopedKeepAlive(
      KeepAliveOrigin::CHROME_APP_DELEGATE, KeepAliveRestartOption::ENABLED));
  EXPECT_EQ(0, on_restart_allowed_call_count_);

  // Now restart should be allowed, the only one left allows it.
  keep_alive.reset();
  ASSERT_EQ(1, on_restart_allowed_call_count_--);

  // No keep alive, we should no prevent restarts.
  keep_alive.reset();
  EXPECT_EQ(0, on_restart_forbidden_call_count_);

  // Make sure all calls were checked.
  EXPECT_EQ(0, on_restart_allowed_call_count_);
  EXPECT_EQ(0, on_restart_forbidden_call_count_);
}
