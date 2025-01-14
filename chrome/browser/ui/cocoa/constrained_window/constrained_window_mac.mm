// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/cocoa/constrained_window/constrained_window_mac.h"

#include <utility>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/constrained_window/constrained_window_sheet.h"
#import "chrome/browser/ui/cocoa/single_web_contents_dialog_manager_cocoa.h"
#include "components/guest_view/browser/guest_view_base.h"
#include "content/public/browser/browser_thread.h"

using web_modal::WebContentsModalDialogManager;

scoped_ptr<ConstrainedWindowMac> CreateAndShowWebModalDialogMac(
    ConstrainedWindowMacDelegate* delegate,
    content::WebContents* web_contents,
    id<ConstrainedWindowSheet> sheet) {
  ConstrainedWindowMac* window =
      new ConstrainedWindowMac(delegate, web_contents, sheet);
  window->ShowWebContentsModalDialog();
  return scoped_ptr<ConstrainedWindowMac>(window);
}

scoped_ptr<ConstrainedWindowMac> CreateWebModalDialogMac(
    ConstrainedWindowMacDelegate* delegate,
    content::WebContents* web_contents,
    id<ConstrainedWindowSheet> sheet) {
  return scoped_ptr<ConstrainedWindowMac>(
      new ConstrainedWindowMac(delegate, web_contents, sheet));
}

ConstrainedWindowMac::ConstrainedWindowMac(
    ConstrainedWindowMacDelegate* delegate,
    content::WebContents* web_contents,
    id<ConstrainedWindowSheet> sheet)
    : delegate_(delegate),
      sheet_([sheet retain]) {
  DCHECK(sheet);

  // |web_contents| may be embedded within a chain of nested GuestViews. If it
  // is, follow the chain of embedders to the outermost WebContents and use it.
  web_contents_ =
      guest_view::GuestViewBase::GetTopLevelWebContents(web_contents);

  native_manager_.reset(
      new SingleWebContentsDialogManagerCocoa(this, sheet_.get(),
                                              GetDialogManager()));
}

ConstrainedWindowMac::~ConstrainedWindowMac() {
  CHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  native_manager_.reset();
  DCHECK(!manager_);
}

void ConstrainedWindowMac::ShowWebContentsModalDialog() {
  scoped_ptr<SingleWebContentsDialogManagerCocoa> dialog_manager;
  dialog_manager.reset(native_manager_.release());
  GetDialogManager()->ShowDialogWithManager(
      [sheet_.get() sheetWindow], std::move(dialog_manager));
}

void ConstrainedWindowMac::CloseWebContentsModalDialog() {
  if (manager_)
    manager_->Close();
}

void ConstrainedWindowMac::OnDialogClosing() {
  if (delegate_)
    delegate_->OnConstrainedWindowClosed(this);
}

bool ConstrainedWindowMac::DialogWasShown() {
  // If the dialog was shown, |native_manager_| would have been released.
  return !native_manager_;
}

WebContentsModalDialogManager* ConstrainedWindowMac::GetDialogManager() {
  DCHECK(web_contents_);
  return WebContentsModalDialogManager::FromWebContents(web_contents_);
}
