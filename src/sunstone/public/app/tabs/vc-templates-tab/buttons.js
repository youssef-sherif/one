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
  var Locale = require('utils/locale');  

  var Buttons = {
    "VCTemplate.refresh" : {
      type: "action",
      layout: "refresh",
      alwaysActive: true
    },
    "VCTemplate.create_dialog" : {
      type: "create_dialog",
      layout: "create"
    },  
    "VCTemplate.update_dialog" : {
      type: "action",
      layout: "main",
      text: Locale.tr("Update")
    },
    "VCTemplate.instantiate_vms" : {
      type: "action",
      layout: "main",
      text: Locale.tr("Instantiate")
    },
    "VCTemplate.chown" : {
      type: "confirm_with_select",
      text: Locale.tr("Change owner"),
      layout: "user_select",
      select: "User",
      tip: Locale.tr("Select the new owner"),
    },
    "VCTemplate.chgrp" : {
      type: "confirm_with_select",
      text: Locale.tr("Change group"),
      layout: "user_select",
      select: "Group",
      tip: Locale.tr("Select the new group"),
    },
    "VCTemplate.clone_dialog" : {
      type: "action",
      layout: "main",
      text: Locale.tr("Clone")
    },
    "VCTemplate.share" : {
      type: "confirm",
      layout: "user_select",
      text: Locale.tr("Share"),
      tip: Locale.tr("The template, along with any image referenced by it, will be shared with the group's users. Permission changed: GROUP USE"),
    },
    "VCTemplate.unshare" : {
      type: "confirm",
      layout: "user_select",
      text: Locale.tr("Unshare"),
      tip: Locale.tr("The template, along with any image referenced by it, will be unshared with the group's users. Permission changed: GROUP USE"),
    },
    "VCTemplate.delete_dialog" : {
      type: "action",
      layout: "del",
      text: Locale.tr("Delete")
    },
    "VCTemplate.edit_labels" : {
      layout: "labels",
    },
    // "Template.lockA" : {
    //   type: "action",
    //   text: Locale.tr("Admin"),
    //   layout: "lock_buttons",
    //   data: 3
    // },
    // "Template.lockM" : {
    //   type: "action",
    //   text: Locale.tr("Manage"),
    //   layout: "lock_buttons",
    //   data: 2
    // },
    "VCTemplate.lockU" : {
      type: "action",
      text: Locale.tr("Lock"),
      layout: "lock_buttons",
      data: 1
    },
    "VCTemplate.unlock" : {
      type: "action",
      text: Locale.tr("Unlock"),
      layout: "lock_buttons"
    }
  };

  return Buttons;
})
