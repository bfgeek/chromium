// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.media.router;

import android.content.Context;
import android.content.IntentFilter;
import android.support.v7.media.MediaRouteDescriptor;
import android.support.v7.media.MediaRouteProvider;
import android.support.v7.media.MediaRouteProviderDescriptor;

import com.google.android.gms.cast.CastMediaControlIntent;

import java.util.ArrayList;

/**
 * A dummy MRP that registers some dummy media sinks to the Android support library, so that these
 * dummy sinks can be discovered and shown in the device selection dialog in media router tests.
 * The Cast app id must be fixed to "CCCCCCCC" so that these sinks can be detected.
 */
final class DummyMediaRouteProvider extends MediaRouteProvider {
    private static final String DUMMY_ROUTE_ID1 = "test_sink_id_1";
    private static final String DUMMY_ROUTE_ID2 = "test_sink_id_2";
    private static final String DUMMY_ROUTE_NAME1 = "test-sink-1";
    private static final String DUMMY_ROUTE_NAME2 = "test-sink-2";

    public DummyMediaRouteProvider(Context context) {
        super(context);

        publishRoutes();
    }

    private void publishRoutes() {
        IntentFilter filter = new IntentFilter();
        filter.addCategory(CastMediaControlIntent.categoryForCast("CCCCCCCC"));
        filter.addDataScheme("http");
        filter.addDataScheme("https");
        filter.addDataScheme("file");

        ArrayList<IntentFilter> controlFilters = new ArrayList<IntentFilter>();
        controlFilters.add(filter);

        MediaRouteDescriptor testRouteDescriptor1 = new MediaRouteDescriptor.Builder(
                DUMMY_ROUTE_ID1, DUMMY_ROUTE_NAME1)
                        .setDescription(DUMMY_ROUTE_NAME1).addControlFilters(controlFilters)
                        .build();
        MediaRouteDescriptor testRouteDescriptor2 = new MediaRouteDescriptor.Builder(
                DUMMY_ROUTE_ID2, DUMMY_ROUTE_NAME2)
                        .setDescription(DUMMY_ROUTE_NAME2).addControlFilters(controlFilters)
                        .build();

        MediaRouteProviderDescriptor providerDescriptor = new MediaRouteProviderDescriptor.Builder()
                .addRoute(testRouteDescriptor1)
                .addRoute(testRouteDescriptor2)
                .build();
        setDescriptor(providerDescriptor);
    }
}
