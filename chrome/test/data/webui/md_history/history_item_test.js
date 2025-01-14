// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('md_history.history_item_test', function() {
  var TEST_HISTORY_RESULTS = [
    {"time": "1000000000"},
    {"time": "100000000"},
    {"time": "9000020"},
    {"time": "9000000"},
    {"time": "1"}
  ];

  var SEARCH_HISTORY_RESULTS = [
    {"dateShort": "Feb 22, 2016", "title": "Search result"},
    {"dateShort": "Feb 21, 2016", "title": "Search result 2"},
    {"dateShort": "Feb 21, 2016", "title": "Search result 3"},
  ];

  function registerTests() {
    suite('history-item', function() {
      var element;

      suiteSetup(function() {
        element = $('history-list');
      });

      setup(function() {
        element.addNewResults(TEST_HISTORY_RESULTS, '');
      });

      test('basic separator insertion', function(done) {
        flush(function() {
          // Check that the correct number of time gaps are inserted.
          var items =
              Polymer.dom(element.root).querySelectorAll('history-item');

          assertTrue(items[0].item.needsTimeGap);
          assertTrue(items[1].item.needsTimeGap);
          assertFalse(items[2].item.needsTimeGap);
          assertTrue(items[3].item.needsTimeGap);
          assertFalse(items[4].item.needsTimeGap);

          done();
        });
      });

      test('separator insertion for search', function(done) {
        element.addNewResults(SEARCH_HISTORY_RESULTS, 'search');
        flush(function() {
          var items =
              Polymer.dom(element.root).querySelectorAll('history-item');

          assertTrue(items[0].item.needsTimeGap);
          assertFalse(items[1].item.needsTimeGap);
          assertFalse(items[2].item.needsTimeGap);

          done();
        });
      });

      test('separator insertion after deletion', function(done) {
        flush(function() {
          var items =
              Polymer.dom(element.root).querySelectorAll('history-item');

          element.set('historyData.3.selected', true);
          items[3].onCheckboxSelected_();

          element.removeDeletedHistory(1);
          assertEquals(element.historyData.length, 4);

          // Checks that a new time gap separator has been inserted.
          assertTrue(items[2].item.needsTimeGap);

          element.set('historyData.3.selected', true);
          items[3].onCheckboxSelected_();
          element.removeDeletedHistory(1);

          // Checks time gap separator is removed.
          assertFalse(items[2].item.needsTimeGap);
          done();
        });
      });

      teardown(function() {
        element.historyData = [];
      });
    });
  }
  return {
    registerTests: registerTests
  };
});
