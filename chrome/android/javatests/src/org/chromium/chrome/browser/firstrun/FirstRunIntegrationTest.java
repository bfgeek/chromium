// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.firstrun;

import android.test.suitebuilder.annotation.SmallTest;
import android.view.KeyEvent;

import org.chromium.base.test.util.CommandLineFlags;
import org.chromium.base.test.util.Feature;
import org.chromium.chrome.browser.ChromeSwitches;
import org.chromium.chrome.browser.tabmodel.TabList;
import org.chromium.chrome.test.ChromeTabbedActivityTestBase;
import org.chromium.content.browser.test.util.Criteria;
import org.chromium.content.browser.test.util.CriteriaHelper;

/**
 * Integration test suite for the first run experience.
 */
@CommandLineFlags.Remove(ChromeSwitches.DISABLE_FIRST_RUN_EXPERIENCE)
public class FirstRunIntegrationTest extends ChromeTabbedActivityTestBase {

    /**
     * Exiting the first run experience should close all tabs,
     * finalize the tab closures and close Chrome.
     */
    @SmallTest
    @Feature({"FirstRunExperience"})
    public void testExitFirstRunExperience() throws InterruptedException {
        if (FirstRunStatus.getFirstRunFlowComplete(getActivity())) {
            return;
        }

        sendKeys(KeyEvent.KEYCODE_BACK);

        CriteriaHelper.pollForCriteria(new Criteria("Expected no tabs to be present") {
            @Override
            public boolean isSatisfied() {
                return 0 == getActivity().getCurrentTabModel().getCount();
            }
        });
        TabList fullModel = getActivity().getCurrentTabModel().getComprehensiveModel();
        assertEquals("Expected no tabs to be present in the comprehensive model",
                0, fullModel.getCount());
        CriteriaHelper.pollForCriteria(new Criteria("Activity was not closed.") {
            @Override
            public boolean isSatisfied() {
                return getActivity().isFinishing() || getActivity().isDestroyed();
            }
        });
    }

    @Override
    public void startMainActivity() throws InterruptedException {
        startMainActivityFromLauncher();
    }

}
