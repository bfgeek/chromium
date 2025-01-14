// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.provider.MediaStore;
import android.test.suitebuilder.annotation.MediumTest;

import org.chromium.base.ThreadUtils;
import org.chromium.base.test.util.DisableIf;
import org.chromium.base.test.util.Feature;
import org.chromium.base.test.util.UrlUtils;
import org.chromium.chrome.test.ChromeActivityTestCaseBase;
import org.chromium.content.browser.ContentViewCore;
import org.chromium.content.browser.test.util.Criteria;
import org.chromium.content.browser.test.util.CriteriaHelper;
import org.chromium.content.browser.test.util.DOMUtils;
import org.chromium.ui.base.ActivityWindowAndroid;
import org.chromium.ui.base.SelectFileDialog;

/**
 * Integration test for select file dialog used for <input type="file" />
 */
public class SelectFileDialogTest extends ChromeActivityTestCaseBase<ChromeActivity> {
    private static final String DATA_URL = UrlUtils.encodeHtmlDataUri(
            "<html><head><meta name=\"viewport\""
            + "content=\"width=device-width, initial-scale=2.0, maximum-scale=2.0\" /></head>"
            + "<body><form action=\"about:blank\">"
            + "<input id=\"input_file\" type=\"file\" /><br/>"
            + "<input id=\"input_file_multiple\" type=\"file\" multiple /><br />"
            + "<input id=\"input_image\" type=\"file\" accept=\"image/*\" capture /><br/>"
            + "<input id=\"input_audio\" type=\"file\" accept=\"audio/*\" capture />"
            + "</form>"
            + "</body></html>");

    private static class ActivityWindowAndroidForTest extends ActivityWindowAndroid {
        public Intent lastIntent;
        public IntentCallback lastCallback;
        /**
         * @param activity
         */
        public ActivityWindowAndroidForTest(Activity activity) {
            super(activity);
        }

        @Override
        public int showCancelableIntent(Intent intent, IntentCallback callback, Integer errorId) {
            lastIntent = intent;
            lastCallback = callback;
            return 1;
        }

        @Override
        public boolean canResolveActivity(Intent intent) {
            return true;
        }
    }

    private class IntentSentCriteria extends Criteria {
        public IntentSentCriteria() {
            super("SelectFileDialog never sent an intent.");
        }

        @Override
        public boolean isSatisfied() {
            return mActivityWindowAndroidForTest.lastIntent != null;
        }
    }

    private ContentViewCore mContentViewCore;
    private ActivityWindowAndroidForTest mActivityWindowAndroidForTest;

    public SelectFileDialogTest() {
        super(ChromeActivity.class);
    }

    @Override
    public void startMainActivity() throws InterruptedException {
        startMainActivityWithURL(DATA_URL);
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();

        mActivityWindowAndroidForTest = new ActivityWindowAndroidForTest(getActivity());
        SelectFileDialog.setWindowAndroidForTests(mActivityWindowAndroidForTest);

        mContentViewCore = getActivity().getCurrentContentViewCore();
        // TODO(aurimas) remove this wait once crbug.com/179511 is fixed.
        assertWaitForPageScaleFactorMatch(2);
        DOMUtils.waitForNonZeroNodeBounds(mContentViewCore.getWebContents(), "input_file");
    }

    /**
     * Tests that clicks on <input type="file" /> trigger intent calls to ActivityWindowAndroid.
     */
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    @MediumTest
    @Feature({"TextInput", "Main"})
    @DisableIf.Build(sdk_is_greater_than = 22, message = "crbug.com/592627")
    public void testSelectFileAndCancelRequest() throws Throwable {
        DOMUtils.clickNode(this, mContentViewCore, "input_file");
        CriteriaHelper.pollForCriteria(new IntentSentCriteria());
        assertEquals(Intent.ACTION_CHOOSER, mActivityWindowAndroidForTest.lastIntent.getAction());
        resetActivityWindowAndroidForTest();

        DOMUtils.clickNode(this, mContentViewCore, "input_file_multiple");
        CriteriaHelper.pollForCriteria(new IntentSentCriteria());
        assertEquals(Intent.ACTION_CHOOSER, mActivityWindowAndroidForTest.lastIntent.getAction());
        Intent contentIntent = (Intent)
                mActivityWindowAndroidForTest.lastIntent.getParcelableExtra(Intent.EXTRA_INTENT);
        assertNotNull(contentIntent);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
            assertTrue(contentIntent.hasExtra(Intent.EXTRA_ALLOW_MULTIPLE));
        }
        resetActivityWindowAndroidForTest();

        DOMUtils.clickNode(this, mContentViewCore, "input_image");
        CriteriaHelper.pollForCriteria(new IntentSentCriteria());
        assertEquals(MediaStore.ACTION_IMAGE_CAPTURE,
                mActivityWindowAndroidForTest.lastIntent.getAction());
        resetActivityWindowAndroidForTest();

        DOMUtils.clickNode(this, mContentViewCore, "input_audio");
        CriteriaHelper.pollForCriteria(new IntentSentCriteria());
        assertEquals(MediaStore.Audio.Media.RECORD_SOUND_ACTION,
                mActivityWindowAndroidForTest.lastIntent.getAction());
        resetActivityWindowAndroidForTest();
    }

    private void resetActivityWindowAndroidForTest() {
        ThreadUtils.runOnUiThreadBlocking(new Runnable() {
            @Override
            public void run() {
                mActivityWindowAndroidForTest.lastCallback.onIntentCompleted(
                        mActivityWindowAndroidForTest, Activity.RESULT_CANCELED, null, null);
            }
        });
        mActivityWindowAndroidForTest.lastCallback = null;
        mActivityWindowAndroidForTest.lastIntent = null;
    }
}
