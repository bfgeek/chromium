// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_CACHE_RENDERER_WEB_CACHE_RENDER_PROCESS_OBSERVER_H_
#define COMPONENTS_WEB_CACHE_RENDERER_WEB_CACHE_RENDER_PROCESS_OBSERVER_H_

#include <stddef.h>
#include <stdint.h>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "components/web_cache/public/interfaces/web_cache.mojom.h"
#include "content/public/renderer/render_process_observer.h"
#include "mojo/public/cpp/bindings/binding_set.h"

namespace web_cache {

// This class implements the Mojo interface mojom::WebCache.
class WebCacheRenderProcessObserver : public mojom::WebCache,
                                      public content::RenderProcessObserver {
 public:
  WebCacheRenderProcessObserver();
  ~WebCacheRenderProcessObserver() override;

  void BindRequest(mojo::InterfaceRequest<mojom::WebCache> web_cache_request);

  // Needs to be called by RenderViews in case of navigations to execute
  // any 'clear cache' commands that were delayed until the next navigation.
  void ExecutePendingClearCache();

 private:
  // RenderProcessObserver implementation.
  void WebKitInitialized() override;
  void OnRenderProcessShutdown() override;

  // mojom::WebCache methods:
  void SetCacheCapacities(uint64_t min_dead_capacity,
                          uint64_t max_dead_capacity,
                          uint64_t capacity) override;
  // If |on_navigation| is true, the clearing is delayed until the next
  // navigation event.
  void ClearCache(bool on_navigation) override;

  // If true, the web cache shall be cleared before the next navigation event.
  bool clear_cache_pending_;
  bool webkit_initialized_;
  size_t pending_cache_min_dead_capacity_;
  size_t pending_cache_max_dead_capacity_;
  size_t pending_cache_capacity_;

  mojo::BindingSet<mojom::WebCache> bindings_;

  DISALLOW_COPY_AND_ASSIGN(WebCacheRenderProcessObserver);
};

}  // namespace web_cache

#endif  // COMPONENTS_WEB_CACHE_RENDERER_WEB_CACHE_RENDER_PROCESS_OBSERVER_H_

