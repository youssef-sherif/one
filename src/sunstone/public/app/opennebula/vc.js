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
    var OpenNebulaAction = require('./action'),
        OpenNebulaHelper = require('./helper'),
        OpenNebulaError  = require('./error');
        Locale = require('utils/locale'),
        Navigation = require('utils/navigation');
      
  
    var RESOURCE = "VC";

    var STATES = {
      INIT            : 0,
      PENDING         : 1,
      HOLD            : 2,
      ACTIVE          : 3,
      STOPPED         : 4,
      SUSPENDED       : 5,
      DONE            : 6,
      //FAILED        : 7,
      POWEROFF        : 8,
      UNDEPLOYED      : 9,
      CLONING         : 10,
      CLONING_FAILURE : 11
    };

    var LCM_STATES = {
      LCM_INIT                        : 0,
      PROLOG                          : 1,
      BOOT                            : 2,
      RUNNING                         : 3,
      MIGRATE                         : 4,
      SAVE_STOP                       : 5,
      SAVE_SUSPEND                    : 6,
      SAVE_MIGRATE                    : 7,
      PROLOG_MIGRATE                  : 8,
      PROLOG_RESUME                   : 9,
      EPILOG_STOP                     : 10,
      EPILOG                          : 11,
      SHUTDOWN                        : 12,
      //CANCEL                        : 13,
      //FAILURE                       : 14,
      CLEANUP_RESUBMIT                : 15,
      UNKNOWN                         : 16,
      HOTPLUG                         : 17,
      SHUTDOWN_POWEROFF               : 18,
      BOOT_UNKNOWN                    : 19,
      BOOT_POWEROFF                   : 20,
      BOOT_SUSPENDED                  : 21,
      BOOT_STOPPED                    : 22,
      CLEANUP_DELETE                  : 23,
      HOTPLUG_SNAPSHOT                : 24,
      HOTPLUG_NIC                     : 25,
      HOTPLUG_SAVEAS                  : 26,
      HOTPLUG_SAVEAS_POWEROFF         : 27,
      HOTPLUG_SAVEAS_SUSPENDED        : 28,
      SHUTDOWN_UNDEPLOY               : 29,
      EPILOG_UNDEPLOY                 : 30,
      PROLOG_UNDEPLOY                 : 31,
      BOOT_UNDEPLOY                   : 32,
      HOTPLUG_PROLOG_POWEROFF         : 33,
      HOTPLUG_EPILOG_POWEROFF         : 34,
      BOOT_MIGRATE                    : 35,
      BOOT_FAILURE                    : 36,
      BOOT_MIGRATE_FAILURE            : 37,
      PROLOG_MIGRATE_FAILURE          : 38,
      PROLOG_FAILURE                  : 39,
      EPILOG_FAILURE                  : 40,
      EPILOG_STOP_FAILURE             : 41,
      EPILOG_UNDEPLOY_FAILURE         : 42,
      PROLOG_MIGRATE_POWEROFF         : 43,
      PROLOG_MIGRATE_POWEROFF_FAILURE : 44,
      PROLOG_MIGRATE_SUSPEND          : 45,
      PROLOG_MIGRATE_SUSPEND_FAILURE  : 46,
      BOOT_UNDEPLOY_FAILURE           : 47,
      BOOT_STOPPED_FAILURE            : 48,
      PROLOG_RESUME_FAILURE           : 49,
      PROLOG_UNDEPLOY_FAILURE         : 50,
      // DISK_SNAPSHOT_POWEROFF          : 51,
      // DISK_SNAPSHOT_REVERT_POWEROFF   : 52,
      // DISK_SNAPSHOT_DELETE_POWEROFF   : 53,
      // DISK_SNAPSHOT_SUSPENDED         : 54,
      // DISK_SNAPSHOT_REVERT_SUSPENDED  : 55,
      // DISK_SNAPSHOT_DELETE_SUSPENDED  : 56,
      // DISK_SNAPSHOT                   : 57,
      // DISK_SNAPSHOT_REVERT          : 58,
      // DISK_SNAPSHOT_DELETE            : 59,
      PROLOG_MIGRATE_UNKNOWN          : 60,
      PROLOG_MIGRATE_UNKNOWN_FAILURE  : 61,
      DISK_RESIZE                     : 62,
      DISK_RESIZE_POWEROFF            : 63,
      DISK_RESIZE_UNDEPLOYED          : 64
    };

    var SHORT_LCM_STATES_STR = [
      Locale.tr("LCM_INIT"),  // LCM_INIT
      Locale.tr("PROLOG"),    // PROLOG
      Locale.tr("BOOT"),      // BOOT
      Locale.tr("RUNNING"),   // RUNNING
      Locale.tr("MIGRATE"),   // MIGRATE
      Locale.tr("SAVE"),      // SAVE_STOP
      Locale.tr("SAVE"),      // SAVE_SUSPEND
      Locale.tr("SAVE"),      // SAVE_MIGRATE
      Locale.tr("MIGRATE"),   // PROLOG_MIGRATE
      Locale.tr("PROLOG"),    // PROLOG_RESUME
      Locale.tr("EPILOG"),    // EPILOG_STOP
      Locale.tr("EPILOG"),    // EPILOG
      Locale.tr("SHUTDOWN"),  // SHUTDOWN
      Locale.tr("SHUTDOWN"),  // CANCEL
      Locale.tr("FAILURE"),   // FAILURE
      Locale.tr("CLEANUP"),   // CLEANUP_RESUBMIT
      Locale.tr("UNKNOWN"),   // UNKNOWN
      Locale.tr("HOTPLUG"),   // HOTPLUG
      Locale.tr("SHUTDOWN"),  // SHUTDOWN_POWEROFF
      Locale.tr("BOOT"),      // BOOT_UNKNOWN
      Locale.tr("BOOT"),      // BOOT_POWEROFF
      Locale.tr("BOOT"),      // BOOT_SUSPENDED
      Locale.tr("BOOT"),      // BOOT_STOPPED
      Locale.tr("CLEANUP"),   // CLEANUP_DELETE
      Locale.tr("SNAPSHOT"),  // HOTPLUG_SNAPSHOT
      Locale.tr("HOTPLUG"),   // HOTPLUG_NIC
      Locale.tr("HOTPLUG"),   // HOTPLUG_SAVEAS
      Locale.tr("HOTPLUG"),   // HOTPLUG_SAVEAS_POWEROFF
      Locale.tr("HOTPLUG"),   // HOTPLUG_SAVEAS_SUSPENDED
      Locale.tr("SHUTDOWN"),  // SHUTDOWN_UNDEPLOY
      Locale.tr("EPILOG"),    // EPILOG_UNDEPLOY
      Locale.tr("PROLOG"),    // PROLOG_UNDEPLOY
      Locale.tr("BOOT"),      // BOOT_UNDEPLOY
      Locale.tr("HOTPLUG"),   // HOTPLUG_PROLOG_POWEROFF
      Locale.tr("HOTPLUG"),   // HOTPLUG_EPILOG_POWEROFF
      Locale.tr("BOOT"),      // BOOT_MIGRATE
      Locale.tr("FAILURE"),   // BOOT_FAILURE
      Locale.tr("FAILURE"),   // BOOT_MIGRATE_FAILURE
      Locale.tr("FAILURE"),   // PROLOG_MIGRATE_FAILURE
      Locale.tr("FAILURE"),   // PROLOG_FAILURE
      Locale.tr("FAILURE"),   // EPILOG_FAILURE
      Locale.tr("FAILURE"),   // EPILOG_STOP_FAILURE
      Locale.tr("FAILURE"),   // EPILOG_UNDEPLOY_FAILURE
      Locale.tr("MIGRATE"),   // PROLOG_MIGRATE_POWEROFF
      Locale.tr("FAILURE"),   // PROLOG_MIGRATE_POWEROFF_FAILURE
      Locale.tr("MIGRATE"),   // PROLOG_MIGRATE_SUSPEND
      Locale.tr("FAILURE"),   // PROLOG_MIGRATE_SUSPEND_FAILURE
      Locale.tr("FAILURE"),   // BOOT_UNDEPLOY_FAILURE
      Locale.tr("FAILURE"),   // BOOT_STOPPED_FAILURE
      Locale.tr("FAILURE"),   // PROLOG_RESUME_FAILURE
      Locale.tr("FAILURE"),   // PROLOG_UNDEPLOY_FAILURE
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_POWEROFF
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_REVERT_POWEROFF
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_DELETE_POWEROFF
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_SUSPENDED
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_REVERT_SUSPENDED
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_DELETE_SUSPENDED
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_REVERT
      // Locale.tr("SNAPSHOT"),  // DISK_SNAPSHOT_DELETE
      Locale.tr("MIGRATE"),   // PROLOG_MIGRATE_UNKNOWN
      Locale.tr("FAILURE"),   // PROLOG_MIGRATE_UNKNOWN_FAILURE
      Locale.tr("DISK_RSZ"),  // DISK_RESIZE
      Locale.tr("DISK_RSZ"),  // DISK_RESIZE_POWEROFF
      Locale.tr("DISK_RSZ")   // DISK_RESIZE_UNDEPLOYED
    ];
  
    var VNC_STATES = [
      LCM_STATES.RUNNING,
      LCM_STATES.MIGRATE,
      LCM_STATES.SHUTDOWN,
      LCM_STATES.CANCEL,
      LCM_STATES.UNKNOWN,
      LCM_STATES.HOTPLUG,
      LCM_STATES.SHUTDOWN_POWEROFF,
      LCM_STATES.HOTPLUG_SNAPSHOT,
      LCM_STATES.HOTPLUG_NIC,
      LCM_STATES.HOTPLUG_SAVEAS,
      LCM_STATES.HOTPLUG_SAVEAS_POWEROFF,
      LCM_STATES.HOTPLUG_SAVEAS_SUSPENDED,
      LCM_STATES.SHUTDOWN_UNDEPLOY,
    ];
  

    var VC = {
        "resource": RESOURCE,
        "create": function(params) {          
          OpenNebulaAction.create(params, RESOURCE);
        },
        "list": function(params) {          
          OpenNebulaAction.list(params, RESOURCE);
        },
        "show": function(params) {
          OpenNebulaAction.show(params, RESOURCE);
        },
        "chown" : function(params) {
          OpenNebulaAction.chown(params, RESOURCE);
        },
        "chgrp" : function(params) {
          OpenNebulaAction.chgrp(params, RESOURCE);
        },
        "chmod" : function(params) {
          var action_obj = params.data.extra_param;
          OpenNebulaAction.simple_action(params, RESOURCE, "chmod", action_obj);
        },
        "terminate": function(params) {
          var action_obj = {"hard": false};
          OpenNebulaAction.simple_action(params, RESOURCE, "terminate", action_obj);
        },
        "terminate_hard" : function(params) {
          var action_obj = {"hard": true};
          OpenNebulaAction.simple_action(params, RESOURCE, "terminate", action_obj);
        },
        "hold": function(params) {
          OpenNebulaAction.simple_action(params, RESOURCE, "hold");
        },
        "release": function(params) {
          OpenNebulaAction.simple_action(params, RESOURCE, "release");
        },
        "stop": function(params) {
          OpenNebulaAction.simple_action(params, RESOURCE, "stop");
        },
        "suspend": function(params) {
          OpenNebulaAction.simple_action(params, RESOURCE, "suspend");
        },
        "resume": function(params) {
          OpenNebulaAction.simple_action(params, RESOURCE, "resume");
        },
        "poweroff" : function(params) {
          var action_obj = {"hard": false};
          OpenNebulaAction.simple_action(params, RESOURCE, "poweroff", action_obj);
        },
        "poweroff_hard" : function(params) {
          var action_obj = {"hard": true};
          OpenNebulaAction.simple_action(params, RESOURCE, "poweroff", action_obj);
        },
        "undeploy" : function(params) {
          var action_obj = {"hard": false};
          OpenNebulaAction.simple_action(params, RESOURCE, "undeploy", action_obj);
        },
        "undeploy_hard" : function(params) {
          var action_obj = {"hard": true};
          OpenNebulaAction.simple_action(params, RESOURCE, "undeploy", action_obj);
        },
        "reboot": function(params) {
          var action_obj = {"hard": false};
          OpenNebulaAction.simple_action(params, RESOURCE, "reboot", action_obj);
        },
        "reboot_hard" : function(params) {
          var action_obj = {"hard": true};
          OpenNebulaAction.simple_action(params, RESOURCE, "reboot", action_obj);
        },
        "log": function(params) {
          OpenNebulaAction.show(params, RESOURCE, "log");
        },
        "deploy": function(params) {
          var action_obj = params.data.extra_param;
          OpenNebulaAction.simple_action(params, RESOURCE, "deploy", action_obj);
        },
        "livemigrate": function(params) {
          var action_obj = params.data.extra_param;
          OpenNebulaAction.simple_action(params, RESOURCE, "livemigrate", action_obj);
        },
        "migrate": function(params) {
          var action_obj = params.data.extra_param;
          OpenNebulaAction.simple_action(params, RESOURCE, "migrate", action_obj);
        },
        "vnc" : function(params, startstop) {
          var callback = params.success;
          var callback_error = params.error;
          var id = params.data.id;
          var resource = RESOURCE;
    
          var method = startstop;
          var action = OpenNebulaHelper.action(method);
          var request = OpenNebulaHelper.request(resource, method, id);
          $.ajax({
            url: "vc/" + id + "/startvnc",
            type: "POST",
            dataType: "json",
            success: function(response) {
              return callback ? callback(request, response) : null;
            },
            error: function(response) {
              return callback_error ?
                  callback_error(request, OpenNebulaError(response)) : null;
            }
          });
        },
        "rename" : function(params) {
          var action_obj = params.data.extra_param;
          OpenNebulaAction.simple_action(params, RESOURCE, "rename", action_obj);
        },
        "stateStr": function(stateId) {
          return STATES_STR[stateId];
        },
        "STATES": STATES,
        "lcmStateStr": function(stateId) {
          return LCM_STATES_STR[stateId];
        },
        "LCM_STATES": LCM_STATES,
        "shortLcmStateStr": function(stateId) {
          return SHORT_LCM_STATES_STR[stateId];
        },
    }

    return VC;
})