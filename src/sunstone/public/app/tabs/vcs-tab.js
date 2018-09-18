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
    var Buttons = require('./vcs-tab/buttons');
    var Actions = require('./vcs-tab/actions');
    var Table = require('./vcs-tab/datatable');
    require('utils/vnc');
    var TAB_ID = require('./vcs-tab/tabId');
    var DATATABLE_ID = "dataTableVcs";
  
    var _dialogs = [
      require('./vcs-tab/dialogs/deploy'),
      require('./vcs-tab/dialogs/migrate'),
      require('./vcs-tab/dialogs/resize'),
      require('./vcs-tab/dialogs/attach-disk'),
      require('./vcs-tab/dialogs/disk-snapshot'),
      require('./vcs-tab/dialogs/disk-saveas'),
      require('./vcs-tab/dialogs/disk-resize'),
      require('./vcs-tab/dialogs/attach-nic'),
      require('./vcs-tab/dialogs/snapshot'),
      require('./vcs-tab/dialogs/revert'),
      require('./vcs-tab/dialogs/vnc'),
      require('./vcs-tab/dialogs/spice'),
      require('./vcs-tab/dialogs/saveas-template')
    ];
  
    var _panelsHooks = [
      require('./vcs-tab/hooks/header'),
      require('./vcs-tab/hooks/state')
    ];
  
    var _panels = [
      require('./vcs-tab/panels/info'),
      require('./vcs-tab/panels/capacity'),
      require('./vcs-tab/panels/storage'),
      require('./vcs-tab/panels/network'),
      require('./vcs-tab/panels/snapshots'),
      require('./vcs-tab/panels/placement'),
      require('./vcs-tab/panels/actions'),
      require('./vcs-tab/panels/conf'),
      require('./vcs-tab/panels/template'),
      require('./vcs-tab/panels/log')
    ];
  
    var _formPanels = [
      require('./vcs-tab/form-panels/create'),
      // require('./vcs-tab/form-panels/updateconf')
    ]
  
    var Tab = {
      tabId: TAB_ID,
      title: Locale.tr("VCs"),
      icon: 'fa-th',
      tabClass: "subTab",
      parentTab: "instances-top-tab",
      listHeader: Locale.tr("VCs"),
      infoHeader: Locale.tr("VC"),
      lockable: true,
      subheader: '<span class="total_vcs"/> <small>' + Locale.tr("TOTAL") + '</small>&emsp;\
          <span class="active_vcs"/> <small>' + Locale.tr("ACTIVE") + '</small>&emsp;\
          <span class="off_vcs"/> <small>' + Locale.tr("OFF") + '</small>&emsp;\
          <span class="pending_vcs"/> <small>' + Locale.tr("PENDING") + '</small>&emsp;\
          <span class="failed_vcs"/> <small>' + Locale.tr("FAILED") + '</small>',
      resource: 'VC',
      buttons: Buttons,
      actions: Actions,
      dataTable: new Table(DATATABLE_ID, {actions: true, info: true}),
      panels: _panels,
      panelsHooks: _panelsHooks,
      formPanels: _formPanels,
      dialogs: _dialogs
    };
  
    return Tab;
  });
  