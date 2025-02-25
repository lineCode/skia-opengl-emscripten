// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * A single action, that can be taken on a set of entries.
 * @interface
 */
class Action {
  /**
   * Executes this action on the set of entries.
   */
  execute() {}

  /**
   * Checks whether this action can execute on the set of entries.
   *
   * @return {boolean} True if the function can execute, false if not.
   */
  canExecute() {}

  /**
   * @return {?string}
   */
  getTitle() {}

  /**
   * Entries that this Action will execute upon.
   * @return {!Array<!Entry|!FileEntry>}
   */
  getEntries() {}
}

/**
 * @typedef {{
 *  alertDialog: FilesAlertDialog,
 *  errorDialog: ErrorDialog,
 *  listContainer: ListContainer,
 * }}
 */
let ActionModelUI;

/** @implements {Action} */
class DriveShareAction {
  /**
   * @param {!Entry} entry
   * @param {!MetadataModel} metadataModel
   * @param {!ActionModelUI} ui
   * @param {!VolumeManager} volumeManager
   */
  constructor(entry, metadataModel, volumeManager, ui) {
    /**
     * @private {!Entry}
     * @const
     */
    this.entry_ = entry;

    /**
     * @private {!MetadataModel}
     * @const
     */
    this.metadataModel_ = metadataModel;

    /**
     * @private {!VolumeManager}
     * @const
     */
    this.volumeManager_ = volumeManager;

    /**
     * @private {!ActionModelUI}
     * @const
     */
    this.ui_ = ui;
  }

  /**
   * @param {!Array<!Entry>} entries
   * @param {!MetadataModel} metadataModel
   * @param {!ActionModelUI} ui
   * @param {!VolumeManager} volumeManager
   * @return {DriveShareAction}
   */
  static create(entries, metadataModel, volumeManager, ui) {
    if (entries.length !== 1) {
      return null;
    }
    return new DriveShareAction(entries[0], metadataModel, volumeManager, ui);
  }

  /**
   * @override
   */
  execute() {
    // Open the Sharing dialog in a new window.
    chrome.fileManagerPrivate.getEntryProperties(
        [this.entry_], ['shareUrl'], results => {
          if (chrome.runtime.lastError) {
            console.error(chrome.runtime.lastError.message);
            return;
          }
          if (results.length != 1) {
            console.error(
                'getEntryProperties for shareUrl should return 1 entry ' +
                '(returned ' + results.length + ')');
            return;
          }
          if (results[0].shareUrl === undefined) {
            console.error('getEntryProperties shareUrl is undefined');
            return;
          }
          util.visitURL(assert(results[0].shareUrl));
        });
  }

  /**
   * @override
   */
  canExecute() {
    const metadata = this.metadataModel_.getCache([this.entry_], ['canShare']);
    assert(metadata.length === 1);
    const canShareItem = metadata[0].canShare !== false;
    return this.volumeManager_.getDriveConnectionState().type !==
        VolumeManagerCommon.DriveConnectionType.OFFLINE &&
        (loadTimeData.getBoolean('DRIVE_FS_ENABLED') ||
         !util.isTeamDriveRoot(this.entry_)) &&
        canShareItem;
  }

  /**
   * @return {?string}
   */
  getTitle() {
    return null;
  }

  /** @override */
  getEntries() {
    return [this.entry_];
  }
}


/** @implements {Action} */
class DriveToggleOfflineAction {
  /**
   * @param {!Array<!Entry>} entries
   * @param {!MetadataModel} metadataModel
   * @param {!DriveSyncHandler} driveSyncHandler
   * @param {!ActionModelUI} ui
   * @param {boolean} value
   * @param {function()} onExecute
   */
  constructor(entries, metadataModel, driveSyncHandler, ui, value, onExecute) {
    /**
     * @private {!Array<!Entry>}
     * @const
     */
    this.entries_ = entries;

    /**
     * @private {!MetadataModel}
     * @const
     */
    this.metadataModel_ = metadataModel;

    /**
     * @private {!DriveSyncHandler}
     * @const
     */
    this.driveSyncHandler_ = driveSyncHandler;

    /**
     * @private {!ActionModelUI}
     * @const
     */
    this.ui_ = ui;

    /**
     * @private {boolean}
     * @const
     */
    this.value_ = value;

    /**
     * @private {function()}
     * @const
     */
    this.onExecute_ = onExecute;
  }

  /**
   * @param {!Array<!Entry>} entries
   * @param {!MetadataModel} metadataModel
   * @param {!DriveSyncHandler} driveSyncHandler
   * @param {!ActionModelUI} ui
   * @param {boolean} value
   * @param {function()} onExecute
   * @return {DriveToggleOfflineAction}
   */
  static create(
      entries, metadataModel, driveSyncHandler, ui, value, onExecute) {
    if (!loadTimeData.getBoolean('DRIVE_FS_ENABLED')) {
      if (entries.some((entry) => entry.isDirectory)) {
        return null;
      }
    }

    const actionableEntries = entries.filter(entry => {
      if (entry.isDirectory && !loadTimeData.getBoolean('DRIVE_FS_ENABLED')) {
        return false;
      }
      const metadata = metadataModel.getCache([entry], ['hosted', 'pinned'])[0];
      if (metadata.hosted) {
        return false;
      }
      if (metadata.pinned === value) {
        return false;
      }
      return true;
    });

    if (actionableEntries.length === 0) {
      return null;
    }

    return new DriveToggleOfflineAction(
        actionableEntries, metadataModel, driveSyncHandler, ui, value,
        onExecute);
  }

  /**
   * @override
   */
  execute() {
    const entries = this.entries_;
    if (entries.length == 0) {
      return;
    }

    let currentEntry;
    let error = false;

    const steps = {
      // Pick an entry and pin it.
      start: () => {
        // Check if all the entries are pinned or not.
        if (entries.length === 0) {
          this.onExecute_();
          return;
        }
        currentEntry = entries.shift();
        chrome.fileManagerPrivate.pinDriveFile(
            currentEntry, this.value_, steps.entryPinned);
      },

      // Check the result of pinning.
      entryPinned: () => {
        error = !!chrome.runtime.lastError;
        if (error && this.value_) {
          this.metadataModel_.get([currentEntry], ['size']).then(results => {
            steps.showError(results[0].size);
          });
          return;
        }
        this.metadataModel_.notifyEntriesChanged([currentEntry]);
        this.metadataModel_.get([currentEntry], ['pinned'])
            .then(steps.updateUI);
      },

      // Update the user interface according to the cache state.
      updateUI: () => {
        this.ui_.listContainer.currentView.updateListItemsMetadata(
            'external', [currentEntry]);
        if (!error) {
          steps.start();
        }
      },

      // Show an error.
      showError: size => {
        this.ui_.alertDialog.showHtml(
            str('DRIVE_OUT_OF_SPACE_HEADER'),
            strf(
                'DRIVE_OUT_OF_SPACE_MESSAGE', unescape(currentEntry.name),
                util.bytesToString(size)),
            null, null, null);
      }
    };
    steps.start();

    if (this.value_ && this.driveSyncHandler_.isSyncSuppressed()) {
      this.driveSyncHandler_.showDisabledMobileSyncNotification();
    }
  }

  /**
   * @override
   */
  canExecute() {
    return true;
  }

  /**
   * @return {?string}
   */
  getTitle() {
    return null;
  }

  /** @override */
  getEntries() {
    return this.entries_;
  }
}


/** @implements {Action} */
class DriveCreateFolderShortcutAction {
  /**
   * @param {!Entry} entry
   * @param {!FolderShortcutsDataModel} shortcutsModel
   * @param {function()} onExecute
   */
  constructor(entry, shortcutsModel, onExecute) {
    /**
     * @private {!Entry}
     * @const
     */
    this.entry_ = entry;

    /**
     * @private {!FolderShortcutsDataModel}
     * @const
     */
    this.shortcutsModel_ = shortcutsModel;

    /**
     * @private {function()}
     * @const
     */
    this.onExecute_ = onExecute;
  }

  /**
   * @param {!Array<!Entry>} entries
   * @param {!VolumeManager} volumeManager
   * @param {!FolderShortcutsDataModel} shortcutsModel
   * @param {function()} onExecute
   * @return {DriveCreateFolderShortcutAction}
   */
  static create(entries, volumeManager, shortcutsModel, onExecute) {
    if (entries.length !== 1 || entries[0].isFile) {
      return null;
    }
    const locationInfo = volumeManager.getLocationInfo(entries[0]);
    if (!locationInfo || locationInfo.isSpecialSearchRoot ||
        locationInfo.isRootEntry) {
      return null;
    }
    return new DriveCreateFolderShortcutAction(
        entries[0], shortcutsModel, onExecute);
  }

  /**
   * @override
   */
  execute() {
    this.shortcutsModel_.add(this.entry_);
    this.onExecute_();
  }

  /**
   * @override
   */
  canExecute() {
    return !this.shortcutsModel_.exists(this.entry_);
  }

  /**
   * @return {?string}
   */
  getTitle() {
    return null;
  }

  /** @override */
  getEntries() {
    return [this.entry_];
  }
}


/** @implements {Action} */
class DriveRemoveFolderShortcutAction {
  /**
   * @param {!Entry} entry
   * @param {!FolderShortcutsDataModel} shortcutsModel
   * @param {function()} onExecute
   */
  constructor(entry, shortcutsModel, onExecute) {
    /**
     * @private {!Entry}
     * @const
     */
    this.entry_ = entry;

    /**
     * @private {!FolderShortcutsDataModel}
     * @const
     */
    this.shortcutsModel_ = shortcutsModel;

    /**
     * @private {function()}
     * @const
     */
    this.onExecute_ = onExecute;
  }

  /**
   * @param {!Array<!Entry>} entries
   * @param {!FolderShortcutsDataModel} shortcutsModel
   * @param {function()} onExecute
   * @return {DriveRemoveFolderShortcutAction}
   */
  static create(entries, shortcutsModel, onExecute) {
    if (entries.length !== 1 || entries[0].isFile ||
        !shortcutsModel.exists(entries[0])) {
      return null;
    }
    return new DriveRemoveFolderShortcutAction(
        entries[0], shortcutsModel, onExecute);
  }

  /**
   * @override
   */
  execute() {
    this.shortcutsModel_.remove(this.entry_);
    this.onExecute_();
  }

  /**
   * @override
   */
  canExecute() {
    return this.shortcutsModel_.exists(this.entry_);
  }

  /**
   * @return {?string}
   */
  getTitle() {
    return null;
  }

  /** @override */
  getEntries() {
    return [this.entry_];
  }
}


/**
 * Opens the entry in Drive Web for the user to manage permissions etc.
 *
 * @implements {Action}
 */
class DriveManageAction {
  /**
   * @param {!Entry} entry The entry to open the 'Manage' page for.
   * @param {!ActionModelUI} ui
   * @param {!VolumeManager} volumeManager
   */
  constructor(entry, volumeManager, ui) {
    /**
     * The entry to open the 'Manage' page for.
     *
     * @private {!Entry}
     * @const
     */
    this.entry_ = entry;

    /**
     * @private {!VolumeManager}
     * @const
     */
    this.volumeManager_ = volumeManager;

    /**
     * @private {!ActionModelUI}
     * @const
     */
    this.ui_ = ui;
  }

  /**
   * Creates a new DriveManageAction object.
   * |entries| must contain only a single entry.
   *
   * @param {!Array<!Entry>} entries
   * @param {!ActionModelUI} ui
   * @param {!VolumeManager} volumeManager
   * @return {DriveManageAction}
   */
  static create(entries, volumeManager, ui) {
    if (entries.length !== 1) {
      return null;
    }

    return new DriveManageAction(entries[0], volumeManager, ui);
  }

  /**
   * @override
   */
  execute() {
    chrome.fileManagerPrivate.getEntryProperties(
        [this.entry_], ['alternateUrl'], results => {
          if (chrome.runtime.lastError) {
            console.error(chrome.runtime.lastError.message);
            return;
          }
          if (results.length != 1) {
            console.error(
                'getEntryProperties for alternateUrl should return 1 entry ' +
                '(returned ' + results.length + ')');
            return;
          }
          if (results[0].alternateUrl === undefined) {
            console.error('getEntryProperties alternateUrl is undefined');
            return;
          }
          util.visitURL(assert(results[0].alternateUrl));
        });
  }

  /**
   * @override
   */
  canExecute() {
    return this.volumeManager_.getDriveConnectionState().type !==
        VolumeManagerCommon.DriveConnectionType.OFFLINE &&
        (loadTimeData.getBoolean('DRIVE_FS_ENABLED') ||
         !util.isTeamDriveRoot(this.entry_));
  }

  /**
   * @return {?string}
   */
  getTitle() {
    return null;
  }

  /** @override */
  getEntries() {
    return [this.entry_];
  }
}


/**
 * A custom action set by the FSP API.
 *
 * @implements {Action}
 */
class CustomAction {
  /**
   * @param {!Array<!Entry>} entries
   * @param {string} id
   * @param {?string} title
   * @param {function()} onExecute
   */
  constructor(entries, id, title, onExecute) {
    /**
     * @private {!Array<!Entry>}
     * @const
     */
    this.entries_ = entries;

    /**
     * @private {string}
     * @const
     */
    this.id_ = id;

    /**
     * @private {?string}
     * @const
     */
    this.title_ = title;

    /**
     * @private {function()}
     * @const
     */
    this.onExecute_ = onExecute;
  }

  /**
   * @override
   */
  execute() {
    chrome.fileManagerPrivate.executeCustomAction(
        this.entries_, this.id_, () => {
          if (chrome.runtime.lastError) {
            console.error(
                'Failed to execute a custom action because of: ' +
                chrome.runtime.lastError.message);
          }
          this.onExecute_();
        });
  }

  /**
   * @override
   */
  canExecute() {
    return true;  // Custom actions are always executable.
  }

  /**
   * @override
   */
  getTitle() {
    return this.title_;
  }

  /** @override */
  getEntries() {
    return this.entries_;
  }
}

/**
 * Represents a set of actions for a set of entries. Includes actions set
 * locally in JS, as well as those retrieved from the FSP API.
 */
class ActionsModel extends cr.EventTarget {
  /**
   * @param {!VolumeManager} volumeManager
   * @param {!MetadataModel} metadataModel
   * @param {!FolderShortcutsDataModel} shortcutsModel
   * @param {!DriveSyncHandler} driveSyncHandler
   * @param {!ActionModelUI} ui
   * @param {!Array<!Entry>} entries
   */
  constructor(
      volumeManager, metadataModel, shortcutsModel, driveSyncHandler, ui,
      entries) {
    super();

    /**
     * @private {!VolumeManager}
     * @const
     */
    this.volumeManager_ = volumeManager;

    /**
     * @private {!MetadataModel}
     * @const
     */
    this.metadataModel_ = metadataModel;

    /**
     * @private {!FolderShortcutsDataModel}
     * @const
     */
    this.shortcutsModel_ = shortcutsModel;

    /**
     * @private {!DriveSyncHandler}
     * @const
     */
    this.driveSyncHandler_ = driveSyncHandler;

    /**
     * @private {!ActionModelUI}
     * @const
     */
    this.ui_ = ui;

    /**
     * @private {!Array<!Entry>}
     * @const
     */
    this.entries_ = entries;

    /**
     * @private {!Object<!Action>}
     */
    this.actions_ = {};

    /**
     * @private {?function()}
     */
    this.initializePromiseReject_ = null;

    /**
     * @private {Promise}
     */
    this.initializePromise_ = null;

    /**
     * @private {boolean}
     */
    this.destroyed_ = false;
  }

  /**
   * Initializes the ActionsModel, including populating the list of available
   * actions for the given entries.
   * @return {!Promise}
   */
  initialize() {
    if (this.initializePromise_) {
      return this.initializePromise_;
    }

    this.initializePromise_ =
        new Promise((fulfill, reject) => {
          if (this.destroyed_) {
            reject();
            return;
          }
          this.initializePromiseReject_ = reject;

          const volumeInfo = this.entries_.length >= 1 &&
              this.volumeManager_.getVolumeInfo(this.entries_[0]);
          // All entries need to be on the same volume to execute ActionsModel
          // commands.
          if (!volumeInfo ||
              !util.isSameVolume(this.entries_, this.volumeManager_)) {
            fulfill({});
            return;
          }

          const actions = {};
          switch (volumeInfo.volumeType) {
            // For Drive, actions are constructed directly in the Files app
            // code.
            case VolumeManagerCommon.VolumeType.DRIVE:
              const shareAction = DriveShareAction.create(
                  this.entries_, this.metadataModel_, this.volumeManager_,
                  this.ui_);
              if (shareAction) {
                actions[ActionsModel.CommonActionId.SHARE] = shareAction;
              }

              const saveForOfflineAction = DriveToggleOfflineAction.create(
                  this.entries_, this.metadataModel_, this.driveSyncHandler_,
                  this.ui_, true, this.invalidate_.bind(this));
              if (saveForOfflineAction) {
                actions[ActionsModel.CommonActionId.SAVE_FOR_OFFLINE] =
                    saveForOfflineAction;
              }

              const offlineNotNecessaryAction = DriveToggleOfflineAction.create(
                  this.entries_, this.metadataModel_, this.driveSyncHandler_,
                  this.ui_, false, this.invalidate_.bind(this));
              if (offlineNotNecessaryAction) {
                actions[ActionsModel.CommonActionId.OFFLINE_NOT_NECESSARY] =
                    offlineNotNecessaryAction;
              }

              const createFolderShortcutAction =
                  DriveCreateFolderShortcutAction.create(
                      this.entries_, this.volumeManager_, this.shortcutsModel_,
                      this.invalidate_.bind(this));
              if (createFolderShortcutAction) {
                actions[ActionsModel.InternalActionId.CREATE_FOLDER_SHORTCUT] =
                    createFolderShortcutAction;
              }

              const removeFolderShortcutAction =
                  DriveRemoveFolderShortcutAction.create(
                      this.entries_, this.shortcutsModel_,
                      this.invalidate_.bind(this));
              if (removeFolderShortcutAction) {
                actions[ActionsModel.InternalActionId.REMOVE_FOLDER_SHORTCUT] =
                    removeFolderShortcutAction;
              }

              const manageInDriveAction = DriveManageAction.create(
                  this.entries_, this.volumeManager_, this.ui_);
              if (manageInDriveAction) {
                actions[ActionsModel.InternalActionId.MANAGE_IN_DRIVE] =
                    manageInDriveAction;
              }

              fulfill(actions);
              break;

            // For FSP, fetch custom actions via an API.
            case VolumeManagerCommon.VolumeType.PROVIDED:
              chrome.fileManagerPrivate.getCustomActions(
                  this.entries_, customActions => {
                    if (chrome.runtime.lastError) {
                      console.error(
                          'Failed to fetch custom actions because of: ' +
                          chrome.runtime.lastError.message);
                    } else {
                      customActions.forEach(action => {
                        actions[action.id] = new CustomAction(
                            this.entries_, action.id, action.title || null,
                            this.invalidate_.bind(this));
                      });
                    }
                    fulfill(actions);
                  });
              break;

            default:
              fulfill(actions);
          }
        }).then(actions => {
          this.actions_ = actions;
        });

    return this.initializePromise_;
  }

  /**
   * @return {!Object<!Action>}
   */
  getActions() {
    return this.actions_;
  }

  /**
   * @param {string} id
   * @return {Action}
   */
  getAction(id) {
    return this.actions_[id] || null;
  }

  /**
   * Destroys the model and cancels initialization if in progress.
   */
  destroy() {
    this.destroyed_ = true;
    if (this.initializePromiseReject_ !== null) {
      const reject = this.initializePromiseReject_;
      this.initializePromiseReject_ = null;
      reject();
    }
  }

  /**
   * Invalidates the current actions model by emitting an invalidation event.
   * The model has to be initialized again, as the list of actions might have
   * changed.
   *
   * @private
   */
  invalidate_() {
    if (this.initializePromiseReject_ !== null) {
      const reject = this.initializePromiseReject_;
      this.initializePromiseReject_ = null;
      this.initializePromise_ = null;
      reject();
    }
    cr.dispatchSimpleEvent(this, 'invalidated', true);
  }

  /**
   * @return {!Array<!Entry>}
   * @public
   */
  getEntries() {
    return this.entries_;
  }
}

/**
 * List of common actions, used both internally and externally (custom actions).
 * Keep in sync with file_system_provider.idl.
 * @enum {string}
 */
ActionsModel.CommonActionId = {
  SHARE: 'SHARE',
  SAVE_FOR_OFFLINE: 'SAVE_FOR_OFFLINE',
  OFFLINE_NOT_NECESSARY: 'OFFLINE_NOT_NECESSARY'
};

/**
 * @enum {string}
 */
ActionsModel.InternalActionId = {
  CREATE_FOLDER_SHORTCUT: 'create-folder-shortcut',
  REMOVE_FOLDER_SHORTCUT: 'remove-folder-shortcut',
  MANAGE_IN_DRIVE: 'manage-in-drive'
};
