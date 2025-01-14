// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/renderer/layout_test/layout_test_render_frame_observer.h"

#include <string>

#include "components/test_runner/layout_dump.h"
#include "components/test_runner/layout_dump_flags.h"
#include "components/test_runner/web_test_interfaces.h"
#include "components/test_runner/web_test_runner.h"
#include "content/public/renderer/render_frame.h"
#include "content/shell/common/shell_messages.h"
#include "content/shell/renderer/layout_test/blink_test_runner.h"
#include "content/shell/renderer/layout_test/layout_test_render_process_observer.h"
#include "ipc/ipc_message_macros.h"
#include "third_party/WebKit/public/web/WebLocalFrame.h"

namespace content {

LayoutTestRenderFrameObserver::LayoutTestRenderFrameObserver(
    RenderFrame* render_frame)
    : RenderFrameObserver(render_frame) {
  render_frame->GetWebFrame()->setContentSettingsClient(
      LayoutTestRenderProcessObserver::GetInstance()
          ->test_interfaces()
          ->TestRunner()
          ->GetWebContentSettings());
}

bool LayoutTestRenderFrameObserver::OnMessageReceived(
    const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(LayoutTestRenderFrameObserver, message)
    IPC_MESSAGE_HANDLER(ShellViewMsg_LayoutDumpRequest, OnLayoutDumpRequest)
    IPC_MESSAGE_HANDLER(ShellViewMsg_ReplicateLayoutDumpFlagsChanges,
                        OnReplicateLayoutDumpFlagsChanges)
    IPC_MESSAGE_HANDLER(ShellViewMsg_ReplicateTestConfiguration,
                        OnReplicateTestConfiguration)
    IPC_MESSAGE_HANDLER(ShellViewMsg_SetTestConfiguration,
                        OnSetTestConfiguration)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}

void LayoutTestRenderFrameObserver::OnLayoutDumpRequest() {
  const test_runner::LayoutDumpFlags& layout_dump_flags =
      LayoutTestRenderProcessObserver::GetInstance()
          ->test_interfaces()
          ->TestRunner()
          ->GetLayoutDumpFlags();
  std::string dump =
      test_runner::DumpLayout(render_frame()->GetWebFrame(), layout_dump_flags);
  Send(new ShellViewHostMsg_LayoutDumpResponse(routing_id(), dump));
}

void LayoutTestRenderFrameObserver::OnReplicateLayoutDumpFlagsChanges(
    const base::DictionaryValue& changed_layout_dump_flags) {
  LayoutTestRenderProcessObserver::GetInstance()
      ->test_interfaces()
      ->TestRunner()
      ->ReplicateLayoutDumpFlagsChanges(changed_layout_dump_flags);
}

void LayoutTestRenderFrameObserver::OnReplicateTestConfiguration(
    const ShellTestConfiguration& test_config,
    const base::DictionaryValue& accumulated_layout_dump_flags_changes) {
  LayoutTestRenderProcessObserver::GetInstance()
      ->main_test_runner()
      ->OnReplicateTestConfiguration(test_config);

  OnReplicateLayoutDumpFlagsChanges(accumulated_layout_dump_flags_changes);
}

void LayoutTestRenderFrameObserver::OnSetTestConfiguration(
    const ShellTestConfiguration& test_config) {
  LayoutTestRenderProcessObserver::GetInstance()
      ->main_test_runner()
      ->OnSetTestConfiguration(test_config);
}

}  // namespace content
