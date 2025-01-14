// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

window.addEventListener('load', function() {
  chrome.send('queryHistory', ['', 0, 0, 0, RESULTS_PER_PAGE]);
  chrome.send('getForeignSessions');
});

/**
 * Listens for history-item being selected or deselected (through checkbox)
 * and changes the view of the top toolbar.
 * @param {{detail: {countAddition: number}}} e
 */
window.addEventListener('history-checkbox-select', function(e) {
  var toolbar = /** @type {HistoryToolbarElement} */($('toolbar'));
  toolbar.count += e.detail.countAddition;
});

/**
 * Listens for call to cancel selection and loops through all items to set
 * checkbox to be unselected.
 */
window.addEventListener('unselect-all', function() {
  var historyList = /** @type {HistoryListElement} */($('history-list'));
  var toolbar = /** @type {HistoryToolbarElement} */($('toolbar'));
  historyList.unselectAllItems(toolbar.count);
  toolbar.count = 0;
});

/**
 * Listens for call to delete all selected items and loops through all items to
 * to determine which ones are selected and deletes these.
 */
window.addEventListener('delete-selected', function() {
  if (!loadTimeData.getBoolean('allowDeletingHistory'))
    return;

  // TODO(hsampson): add a popup to check whether the user definitely wants to
  // delete the selected items.

  var historyList = /** @type {HistoryListElement} */($('history-list'));
  var toolbar = /** @type {HistoryToolbarElement} */($('toolbar'));
  var toBeRemoved = historyList.getSelectedItems(toolbar.count);
  chrome.send('removeVisits', toBeRemoved);
});

/**
 * When the search is changed refresh the results from the backend.
 */
window.addEventListener('search-changed', function(e) {
  /** @type {HistoryListElement} */($('history-list')).setLoading();
  chrome.send('queryHistory', [e.detail.search, 0, 0, 0, RESULTS_PER_PAGE]);
});

/**
 * Switches between displaying history data and synced tabs data for the page.
 */
window.addEventListener('switch-display', function(e) {
  $('history-synced-device-manager').hidden =
      e.detail.display != 'synced-tabs-button';
  $('history-list').hidden = e.detail.display != 'history-button';
});

// Chrome Callbacks-------------------------------------------------------------

/**
 * Our history system calls this function with results from searches.
 * @param {HistoryQuery} info An object containing information about the query.
 * @param {!Array<HistoryEntry>} results A list of results.
 */
function historyResult(info, results) {
  var historyList = /** @type {HistoryListElement} */($('history-list'));
  historyList.addNewResults(results, info.term);
  if (info.finished)
    historyList.disableResultLoading();
}

/**
 * Receives the synced history data. An empty list means that either there are
 * no foreign sessions, or tab sync is disabled for this profile.
 * |isTabSyncEnabled| makes it possible to distinguish between the cases.
 *
 * @param {!Array<!ForeignSession>} sessionList Array of objects describing the
 *     sessions from other devices.
 * @param {boolean} isTabSyncEnabled Is tab sync enabled for this profile?
 */
function setForeignSessions(sessionList, isTabSyncEnabled) {
  // TODO(calamity): Add a 'no synced devices' message when sessions are empty.
  $('history-side-bar').hidden = !isTabSyncEnabled;
  if (isTabSyncEnabled) {
    var syncedDeviceManager = /** @type {HistorySyncedDeviceManagerElement} */(
        $('history-synced-device-manager'));
    syncedDeviceManager.addSyncedHistory(sessionList);
  }
}

/**
 * Called by the history backend when deletion was succesful.
 */
function deleteComplete() {
  var historyList = /** @type {HistoryListElement} */($('history-list'));
  var toolbar = /** @type {HistoryToolbarElement} */($('toolbar'));
  historyList.removeDeletedHistory(toolbar.count);
  toolbar.count = 0;
}

/**
 * Called by the history backend when the deletion failed.
 */
function deleteFailed() {
}

/**
 * Called when the history is deleted by someone else.
 */
function historyDeleted() {
}
