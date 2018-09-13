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
  var OpenNebulaVC = require('opennebula/vc');

  var STATE_ACTIONS = {};

  STATE_ACTIONS[OpenNebulaVC.STATES.INIT] =
    ["VC.resize", "VC.terminate_hard", "VC.recover"];

  STATE_ACTIONS[OpenNebulaVC.STATES.PENDING] =
    ["VC.hold", "VC.deploy", "VC.updateconf", "VC.terminate_hard", "VC.recover", "VC.resize"];

  STATE_ACTIONS[OpenNebulaVC.STATES.HOLD] =
    ["VC.release", "VC.deploy", "VC.updateconf", "VC.terminate_hard", "VC.recover", "VC.resize"];

  STATE_ACTIONS[OpenNebulaVC.STATES.ACTIVE] =
    ["VC.recover"];

  STATE_ACTIONS[OpenNebulaVC.STATES.STOPPED] =
    ["VC.resume", "VC.deploy", "VC.terminate_hard", "VC.recover"];

  STATE_ACTIONS[OpenNebulaVC.STATES.SUSPENDED] =
    ["VC.resume", "VC.disk_saveas", "VC.disk_snapshot_create", "VC.disk_snapshot_revert", "VC.disk_snapshot_delete", "VC.stop", "VC.terminate_hard", "VC.recover", "VC.migrate"];

  STATE_ACTIONS[OpenNebulaVC.STATES.DONE] =
    [];

  STATE_ACTIONS[OpenNebulaVC.STATES.FAILED] =
    [];

  STATE_ACTIONS[OpenNebulaVC.STATES.POWEROFF] =
    ["VC.resume", "VC.resize", "VC.attachdisk", "VC.detachdisk", "VC.attachnic", "VC.detachnic", "VC.disk_saveas", "VC.disk_snapshot_create", "VC.disk_snapshot_revert", "VC.disk_snapshot_delete", "VC.migrate", "VC.undeploy", "VC.undeploy_hard", "VC.save_as_template", "VC.updateconf", "VC.terminate_hard", "VC.recover", "VC.disk_resize", "VC.snapshot_delete"];

  STATE_ACTIONS[OpenNebulaVC.STATES.UNDEPLOYED] =
    ["VC.resume", "VC.resize", "VC.deploy", "VC.updateconf", "VC.terminate_hard", "VC.recover"];

  STATE_ACTIONS[OpenNebulaVC.STATES.CLONING] =
    ["VC.updateconf", "VC.terminate_hard", "VC.recover", "VC.resize"];

  STATE_ACTIONS[OpenNebulaVC.STATES.CLONING_FAILURE] =
    ["VC.updateconf", "VC.terminate_hard", "VC.recover", "VC.resize"];

  var LCM_STATE_ACTIONS = {};
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.LCM_INIT ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.RUNNING ] =
    ["VC.stop", "VC.suspend", "VC.reboot", "VC.reboot_hard", "VC.resched", "VC.unresched", "VC.poweroff", "VC.poweroff_hard", "VC.undeploy", "VC.undeploy_hard", "VC.migrate", "VC.migrate_live", "VC.attachdisk", "VC.detachdisk", "VC.attachnic", "VC.detachnic", "VC.disk_saveas", "VC.disk_snapshot_create", "VC.disk_snapshot_delete", "VC.terminate", "VC.terminate_hard", "VC.disk_resize"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.MIGRATE ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SAVE_STOP ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SAVE_SUSPEND ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SAVE_MIGRATE ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_RESUME ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG_STOP ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SHUTDOWN ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.CANCEL ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.CLEANUP_RESUBMIT ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.UNKNOWN ] =
    ["VC.resched", "VC.unresched", "VC.poweroff", "VC.poweroff_hard", "VC.undeploy", "VC.undeploy_hard", "VC.migrate", "VC.migrate_live", "VC.resume", "VC.terminate", "VC.terminate_hard"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SHUTDOWN_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_UNKNOWN ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_POWEROFF ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_SUSPENDED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_STOPPED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.CLEANUP_DELETE ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_SNAPSHOT ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_NIC ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_SAVEAS ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_SAVEAS_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_SAVEAS_SUSPENDED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.SHUTDOWN_UNDEPLOY ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG_UNDEPLOY ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_UNDEPLOY ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_UNDEPLOY ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_PROLOG_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.HOTPLUG_EPILOG_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_MIGRATE ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_MIGRATE_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG_STOP_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.EPILOG_UNDEPLOY_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_POWEROFF_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_SUSPEND ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_SUSPEND_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_UNDEPLOY_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.BOOT_STOPPED_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_RESUME_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_UNDEPLOY_FAILURE ] = ["VC.updateconf", "VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_REVERT_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_DELETE_POWEROFF ] = ["VC.updateconf"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_SUSPENDED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_REVERT_SUSPENDED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_DELETE_SUSPENDED ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_REVERT ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_SNAPSHOT_DELETE ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_UNKNOWN ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.PROLOG_MIGRATE_UNKNOWN_FAILURE ] = ["VC.terminate"];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_RESIZE            ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_RESIZE_POWEROFF   ] = [];
  LCM_STATE_ACTIONS[ OpenNebulaVC.LCM_STATES.DISK_RESIZE_UNDEPLOYED ] = [];

  return {
    'disableAllStateActions': disableAllStateActions,
    'resetStateButtons': resetStateButtons,
    'enableStateButton': enableStateButton,
    'enableStateActions': enableStateActions,
    'enabledStateAction': enabledStateAction
  };

  function disableAllStateActions() {
    $(".state-dependent").prop("disabled", true).
        removeClass("VC-action-enabled").
        addClass("VC-action-disabled").
        on("click.stateaction", function(e) { return false; });
  }

  function resetStateButtons() {
    $(".state-dependent").
        addClass("VC-action-enabled").
        removeClass("VC-action-disabled").
        off("click.stateaction");
  }

  function enableStateButton(button_action) {
    $(".state-dependent[href='" + button_action + "']").removeAttr("disabled").
        addClass("VC-action-enabled").
        removeClass("VC-action-disabled").
        off("click.stateaction");
  }

  // state and lcm_state are numeric
  function enableStateActions(state, lcm_state) {
    var state = parseInt(state);
    var lcm_state = parseInt(lcm_state);

    $.each(STATE_ACTIONS[state], function(i, action) {
      enableStateButton(action);
    });

    if (state == OpenNebulaVC.STATES.ACTIVE) {
      $.each(LCM_STATE_ACTIONS[lcm_state], function(i, action) {
        enableStateButton(action);
      });
    }
  }

  // Returns true if the action is enabled for the given state
  // action is "VC.action", state and lcm_state are numeric
  function enabledStateAction(action, state, lcm_state) {
    var state = parseInt(state);
    var lcm_state = parseInt(lcm_state);

    return (STATE_ACTIONS[state].indexOf(action) != -1 ||
             (state == OpenNebulaVC.STATES.ACTIVE &&
                LCM_STATE_ACTIONS[lcm_state].indexOf(action) != -1));
  }
});
