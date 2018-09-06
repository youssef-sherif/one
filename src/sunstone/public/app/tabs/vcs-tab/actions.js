/* -------------------------------------------------------------------------- */
/* Copyright 2002-2018, OpenNebula Project, OpenNebula Systems                */
/*                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"); you may    */
/* not use this file except in compliance with the License. You may obtain    */
/* a copy of the License at                                                   */
/*                                                                            */
/* http://www.apache.org/licenses/LICENSE-2.0                                 */
/*                                                                            */
/* Unless required by applicable law or agreed to in writing, software        */
/* distributed under the License is distributed on an "AS IS" BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */
/* See the License for the specific language governing permissions and        */
/* limitations under the License.                                             */
/* -------------------------------------------------------------------------- */

define(function(require) {
  var Config = require('sunstone-config');
  var Sunstone = require('sunstone');
  var Notifier = require('utils/notifier');
  var Locale = require('utils/locale');
  var OpenNebulaVC = require('opennebula/vc');
  var CommonActions = require('utils/common-actions');
  var Vnc = require('utils/vnc');
  var Spice = require('utils/spice');

  var TAB_ID = require('./tabId');
  var CREATE_DIALOG_ID           = require('./form-panels/create/formPanelId');
  // var DEPLOY_DIALOG_ID           = require('./dialogs/deploy/dialogId');
  // var MIGRATE_DIALOG_ID          = require('./dialogs/migrate/dialogId');
  // var VNC_DIALOG_ID              = require('./dialogs/vnc/dialogId');
  // var SPICE_DIALOG_ID            = require('./dialogs/spice/dialogId');  
  // var UPDATECONF_FORM_ID         = require('./form-panels/updateconf/formPanelId');

  var XML_ROOT = "VC";
  var RESOURCE = "VC";

  var _commonActions = new CommonActions(OpenNebulaVC, RESOURCE, TAB_ID,
    XML_ROOT, Locale.tr("VC created"));

  var _actions = {
    "VC.list":    _commonActions.list(),
    "VC.show": {
      type: "single",
      call: OpenNebulaVC.show,
      callback: function(request, response) {
        if (Config.isTabEnabled("provision-tab")) {
          $(".provision_refresh_info", ".provision_list_vms").click();
        } else {
          Sunstone.getDataTable(TAB_ID).updateElement(request, response);
          if (Sunstone.rightInfoVisible($('#' + TAB_ID))) {
            Sunstone.insertPanels(TAB_ID, response);
          }
        }
      },
      error: Notifier.onError
    },
    "VC.refresh": _commonActions.refresh(),
    "VC.chown": _commonActions.multipleAction('chown'),
    "VC.chgrp": _commonActions.multipleAction('chgrp'),
    "VC.hold":    _commonActions.multipleAction('hold'),
    "VC.release": _commonActions.multipleAction('release'),
    "VC.suspend": _commonActions.multipleAction('suspend'),
    "VC.resume": _commonActions.multipleAction('resume'),
    "VC.stop": _commonActions.multipleAction('stop'),
    "VC.reboot": _commonActions.multipleAction('reboot'),
    "VC.reboot_hard": _commonActions.multipleAction('reboot_hard'),
    "VC.poweroff": _commonActions.multipleAction('poweroff'),
    "VC.poweroff_hard": _commonActions.multipleAction('poweroff_hard'),
    "VC.undeploy": _commonActions.multipleAction('undeploy'),
    "VC.undeploy_hard": _commonActions.multipleAction('undeploy_hard'),
    "VC.terminate": _commonActions.multipleAction('terminate'),
    "VC.terminate_hard": _commonActions.multipleAction('terminate_hard'),
    "VC.recover": _commonActions.multipleAction('recover'),
    "VC.resched": _commonActions.multipleAction('resched'),
    "VC.unresched": _commonActions.multipleAction('unresched'),
    "VC.lockM": _commonActions.multipleAction('lock', false),
    "VC.lockU": _commonActions.multipleAction('lock', false),
    "VC.lockA": _commonActions.multipleAction('lock', false),
    "VC.unlock": _commonActions.multipleAction('unlock', false),

    "VC.chmod": _commonActions.singleAction('chmod'),
    "VC.rename": _commonActions.singleAction('rename'),
    "VC.update_template": _commonActions.updateTemplate(),
    "VC.append_template": _commonActions.appendTemplate(),
    "VC.deploy_action": _commonActions.singleAction('deploy'),
    "VC.migrate_action": _commonActions.singleAction('migrate'),
    "VC.migrate_live_action": _commonActions.singleAction('livemigrate'),
    "VC.attachdisk": _commonActions.singleAction('attachdisk'),
    "VC.detachdisk": _commonActions.singleAction('detachdisk'),
    "VC.attachnic": _commonActions.singleAction('attachnic'),
    "VC.detachnic": _commonActions.singleAction('detachnic'),
    "VC.resize": _commonActions.singleAction('resize'),
    "VC.disk_resize": _commonActions.singleAction('disk_resize'),
    "VC.snapshot_create": _commonActions.singleAction('snapshot_create'),
    "VC.snapshot_revert": _commonActions.singleAction('snapshot_revert'),
    "VC.snapshot_delete": _commonActions.singleAction('snapshot_delete'),

    "VC.create_dialog" : {
      type: "custom",
      call: function() {
        Sunstone.showFormPanel(TAB_ID, CREATE_DIALOG_ID, "create");
      }
    },
    "VC.create" : {
      type: "custom",
      call: function(id, name) {
        Sunstone.runAction("VC.instantiate", [id], name);
      },
      callback: function(request, response) {
        Sunstone.runAction("VC.refresh");
      },
      error: Notifier.onError
    },
    // "VC.deploy" : {
    //   type: "custom",
    //   call: function() {
    //     Sunstone.getDialog(DEPLOY_DIALOG_ID).show();
    //   }
    // },
    // "VC.silent_deploy_action" : {
    //   type: "single",
    //   call: OpenNebulaVC.deploy,
    //   error: Notifier.onError
    // },
    // "VC.migrate" : {
    //   type: "custom",
    //   call: function() {
    //    var dialog = Sunstone.getDialog(MIGRATE_DIALOG_ID);
    //    dialog.reset();
    //    dialog.setLive(false);
    //    dialog.show();
    //  }
    // },
    // "VC.migrate_live" : {
    //   type: "custom",
    //   call: function() {
    //    var dialog = Sunstone.getDialog(MIGRATE_DIALOG_ID);
    //    dialog.reset();
    //    dialog.setLive(true);
    //    dialog.show();
    //  }
    // },
    // "VC.startvnc" : {
    //   type: "custom",
    //   call: function() {
    //     $.each(Sunstone.getDataTable(TAB_ID).elements(), function(index, elem) {
    //       if (!Vnc.lockStatus()) {
    //         Vnc.lock();
    //         Sunstone.runAction("VC.startvnc_action", elem);
    //       } else {
    //         Notifier.notifyError(Locale.tr("VNC Connection in progress"))
    //         return false;
    //       }
    //     });
    //   }
    // },
    // "VC.startvnc_action" : {
    //   type: "single",
    //   call: OpenNebulaVC.vnc,
    //   callback: function(request, response) {
    //    var dialog = Sunstone.getDialog(VNC_DIALOG_ID);
    //    dialog.setElement(response);
    //    dialog.show();
    //   },
    //   error: function(req, resp) {
    //     Notifier.onError(req, resp);
    //     Vnc.unlock();
    //   },
    //   notify: true
    // },
    // "VC.startspice" : {
    //   type: "custom",
    //   call: function() {
    //     $.each(Sunstone.getDataTable(TAB_ID).elements(), function(index, elem) {
    //       if (!Spice.lockStatus()) {
    //         Spice.lock();
    //         Sunstone.runAction("VC.startspice_action", elem);
    //       } else {
    //         Notifier.notifyError(Locale.tr("SPICE Connection in progress"))
    //         return false;
    //       }
    //     });
    //   }
    // },
    // "VC.startspice_action" : {
    //   type: "single",
    //   call: OpenNebulaVC.vnc,
    //   callback: function(request, response) {
    //    var dialog = Sunstone.getDialog(SPICE_DIALOG_ID);
    //    dialog.setElement(response);
    //    dialog.show();
    //   },
    //   error: function(req, resp) {
    //     Notifier.onError(req, resp);
    //     Spice.unlock();
    //   },
    //   notify: true
    // },
    // "VC.updateconf": {
    //   type: "single",
    //   call: OpenNebulaVC.updateconf,
    //   callback: function (req) {
    //     Sunstone.resetFormPanel(TAB_ID, UPDATECONF_FORM_ID);
    //     Sunstone.hideFormPanel(TAB_ID);

    //     Sunstone.runAction("VC.refresh");
    //   },
    //   elements: function(opts) {
    //     return Sunstone.getDataTable(TAB_ID).elements(opts);
    //   },
    //   error: function(request, response) {
    //     Sunstone.hideFormPanelLoading(TAB_ID);
    //     Notifier.onError(request, response);
    //   },
    //   notify: false
    // }
  };

  return _actions;
});
