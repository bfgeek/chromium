// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/tuple.h"
#include "content/browser/media/media_web_contents_observer.h"
#include "content/browser/media/session/media_session.h"
#include "content/browser/media/session/media_session_controller.h"
#include "content/common/media/media_player_delegate_messages.h"
#include "content/test/test_render_view_host.h"
#include "content/test/test_web_contents.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

class MediaSessionControllerTest : public RenderViewHostImplTestHarness {
 public:
  void SetUp() override {
    RenderViewHostImplTestHarness::SetUp();
    id_ = WebContentsObserver::MediaPlayerId(contents()->GetMainFrame(), 0);
    controller_ = CreateController();
  }

  void TearDown() override {
    // Destruct the controller prior to any other teardown to avoid out of order
    // destruction relative to the MediaSession instance.
    controller_.reset();
    RenderViewHostImplTestHarness::TearDown();
  }

 protected:
  scoped_ptr<MediaSessionController> CreateController() {
    return scoped_ptr<MediaSessionController>(new MediaSessionController(
        id_, contents()->media_web_contents_observer()));
  }

  MediaSession* media_session() { return MediaSession::Get(contents()); }

  IPC::TestSink& test_sink() { return main_test_rfh()->GetProcess()->sink(); }

  void Suspend() {
    controller_->OnSuspend(controller_->get_player_id_for_testing());
  }

  void Resume() {
    controller_->OnResume(controller_->get_player_id_for_testing());
  }

  void SetVolumeMultiplier(double multiplier) {
    controller_->OnSetVolumeMultiplier(controller_->get_player_id_for_testing(),
                                       multiplier);
  }

  // Returns a duration long enough for a media session instance to be created.
  base::TimeDelta DurationJustRight() {
    return base::TimeDelta::FromSeconds(
        MediaSessionController::kMinimumDurationForContentSecs + 1);
  }

  // Returns a duration too short for a media session instance.
  base::TimeDelta DurationTooShort() {
    return base::TimeDelta::FromSeconds(
        MediaSessionController::kMinimumDurationForContentSecs);
  }

  template <typename T>
  bool ReceivedMessagePlayPause() {
    const IPC::Message* msg = test_sink().GetUniqueMessageMatching(T::ID);
    if (!msg)
      return false;

    base::Tuple<int> result;
    if (!T::Read(msg, &result))
      return false;

    EXPECT_EQ(id_.second, base::get<0>(result));
    test_sink().ClearMessages();
    return id_.second == base::get<0>(result);
  }

  template <typename T>
  bool ReceivedMessageVolumeMultiplierUpdate(double expected_multiplier) {
    const IPC::Message* msg = test_sink().GetUniqueMessageMatching(T::ID);
    if (!msg)
      return false;

    base::Tuple<int, double> result;
    if (!T::Read(msg, &result))
      return false;

    EXPECT_EQ(id_.second, base::get<0>(result));
    if (id_.second != base::get<0>(result))
      return false;

    EXPECT_EQ(expected_multiplier, base::get<1>(result));
    test_sink().ClearMessages();
    return expected_multiplier == base::get<1>(result);
  }

  WebContentsObserver::MediaPlayerId id_;
  scoped_ptr<MediaSessionController> controller_;
};

TEST_F(MediaSessionControllerTest, NoAudioNoSession) {
  ASSERT_TRUE(controller_->Initialize(false, false, DurationJustRight()));
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
}

TEST_F(MediaSessionControllerTest, IsRemoteNoSession) {
  ASSERT_TRUE(controller_->Initialize(true, true, DurationJustRight()));
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
}

TEST_F(MediaSessionControllerTest, TooShortNoControllableSession) {
  ASSERT_TRUE(controller_->Initialize(true, false, DurationTooShort()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
}

TEST_F(MediaSessionControllerTest, BasicControls) {
  ASSERT_TRUE(controller_->Initialize(true, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());

  // Verify suspend notifies the renderer and maintains its session.
  Suspend();
  EXPECT_TRUE(ReceivedMessagePlayPause<MediaPlayerDelegateMsg_Pause>());

  // Likewise verify the resume behavior.
  Resume();
  EXPECT_TRUE(ReceivedMessagePlayPause<MediaPlayerDelegateMsg_Play>());

  // Verify destruction of the controller removes its session.
  controller_.reset();
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
}

TEST_F(MediaSessionControllerTest, VolumeMultiplier) {
  ASSERT_TRUE(controller_->Initialize(true, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());

  // Upon creation of the MediaSession the default multiplier will be sent.
  EXPECT_TRUE(ReceivedMessageVolumeMultiplierUpdate<
              MediaPlayerDelegateMsg_UpdateVolumeMultiplier>(1.0));

  // Verify a different volume multiplier is sent.
  const double kTestMultiplier = 0.5;
  SetVolumeMultiplier(kTestMultiplier);
  EXPECT_TRUE(ReceivedMessageVolumeMultiplierUpdate<
              MediaPlayerDelegateMsg_UpdateVolumeMultiplier>(kTestMultiplier));
}

TEST_F(MediaSessionControllerTest, ControllerSidePause) {
  ASSERT_TRUE(controller_->Initialize(true, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());

  // Verify pause behavior.
  controller_->OnPlaybackPaused();
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());

  // Verify the next Initialize() call restores the session.
  ASSERT_TRUE(controller_->Initialize(true, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());
}

TEST_F(MediaSessionControllerTest, Reinitialize) {
  ASSERT_TRUE(controller_->Initialize(false, false, DurationJustRight()));
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());

  // Create a transient type session.
  ASSERT_TRUE(controller_->Initialize(true, false, DurationTooShort()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
  const int current_player_id = controller_->get_player_id_for_testing();

  // Reinitialize the session as a content type.
  ASSERT_TRUE(controller_->Initialize(true, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());
  // Player id should not change when there's an active session.
  EXPECT_EQ(current_player_id, controller_->get_player_id_for_testing());

  // Verify suspend notifies the renderer and maintains its session.
  Suspend();
  EXPECT_TRUE(ReceivedMessagePlayPause<MediaPlayerDelegateMsg_Pause>());

  // Likewise verify the resume behavior.
  Resume();
  EXPECT_TRUE(ReceivedMessagePlayPause<MediaPlayerDelegateMsg_Play>());

  // Attempt to switch to no audio player, which should do nothing.
  // TODO(dalecurtis): Delete this test once we're no longer using WMPA and
  // the BrowserMediaPlayerManagers.  Tracked by http://crbug.com/580626
  ASSERT_TRUE(controller_->Initialize(false, false, DurationJustRight()));
  EXPECT_FALSE(media_session()->IsSuspended());
  EXPECT_TRUE(media_session()->IsControllable());
  EXPECT_EQ(current_player_id, controller_->get_player_id_for_testing());

  // Switch to a remote player, which should release the session.
  ASSERT_TRUE(controller_->Initialize(true, true, DurationJustRight()));
  EXPECT_TRUE(media_session()->IsSuspended());
  EXPECT_FALSE(media_session()->IsControllable());
  EXPECT_EQ(current_player_id, controller_->get_player_id_for_testing());
}

}  // namespace content
