// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/media/session/media_session_controllers_manager.h"

#include "base/command_line.h"
#include "content/browser/media/session/media_session.h"
#include "content/browser/media/session/media_session_controller.h"
#include "content/browser/media/session/media_session_observer.h"
#include "media/base/media_switches.h"

namespace content {

namespace {

bool IsDefaultMediaSessionEnabled() {
#if defined(OS_ANDROID)
  return true;
#else
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableDefaultMediaSession);
#endif
}

}  // anonymous namespace

MediaSessionControllersManager::MediaSessionControllersManager(
    MediaWebContentsObserver* media_web_contents_observer)
    : media_web_contents_observer_(media_web_contents_observer) {
}

MediaSessionControllersManager::~MediaSessionControllersManager() = default;

void MediaSessionControllersManager::RenderFrameDeleted(
    RenderFrameHost* render_frame_host) {
  if (!IsDefaultMediaSessionEnabled())
    return;

  for (auto it = controllers_map_.begin(); it != controllers_map_.end();) {
    if (it->first.first == render_frame_host)
      it = controllers_map_.erase(it);
    else
      ++it;
  }
}

bool MediaSessionControllersManager::RequestPlay(const MediaPlayerId& id,
    bool has_audio, bool is_remote, base::TimeDelta duration) {
  if (!IsDefaultMediaSessionEnabled())
    return true;

  // Since we don't remove session instances on pause, there may be an existing
  // instance for this playback attempt.
  //
  // In this case, try to reinitialize it with the new settings.  If they are
  // the same, this is a no-op.  If the reinitialize fails, destroy the
  // controller. A later playback attempt will create a new controller.
  auto it = controllers_map_.find(id);
  if (it != controllers_map_.end()) {
    if (it->second->Initialize(has_audio, is_remote, duration))
      return true;
    controllers_map_.erase(it);
    return false;
  }

  scoped_ptr<MediaSessionController> controller(
      new MediaSessionController(id, media_web_contents_observer_));

  if (!controller->Initialize(has_audio, is_remote, duration))
    return false;

  controllers_map_[id] = std::move(controller);
  return true;
}

void MediaSessionControllersManager::OnPause(const MediaPlayerId& id) {
  if (!IsDefaultMediaSessionEnabled())
    return;

  auto it = controllers_map_.find(id);
  if (it == controllers_map_.end())
    return;

  it->second->OnPlaybackPaused();
}

void MediaSessionControllersManager::OnEnd(const MediaPlayerId& id) {
  if (!IsDefaultMediaSessionEnabled())
    return;
  controllers_map_.erase(id);
}

}  // namespace content
