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
        // OpenNebulaHelper = require('./helper'),
        // OpenNebulaError  = require('./error');
        Locale = require('utils/locale'),
        Navigation = require('utils/navigation');
      
  
    var RESOURCE = "VC";

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
    }

    return VC;
})