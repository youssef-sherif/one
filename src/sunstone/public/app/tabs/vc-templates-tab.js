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
    var Buttons = require('./vc-templates-tab/buttons');
    var Actions = require('./vc-templates-tab/actions');
    var Table = require('./vc-templates-tab/datatable');
  
    var TAB_ID = require('./vc-templates-tab/tabId');
    var DATATABLE_ID = "dataTableVCTemplates";
  
    var _dialogs = [
      require('./vc-templates-tab/dialogs/clone')
    ];
  
    // var _panels = [
    //   require('./vc-templates-tab/panels/info'),
    //   require('./vc-templates-tab/panels/template')
    // ];
  
    // var _panelsHooks = [
    //   require('../utils/hooks/header')
    // ];
  
    var _formPanels = [
      require('./vc-templates-tab/form-panels/create'),
      require('./vc-templates-tab/form-panels/instantiate')
    ];
  
    var Tab = {
      tabId: TAB_ID,
      title: Locale.tr("VCs"),
      icon: 'fa-copy',
      tabClass: "subTab",
      parentTab: "templates-top-tab",
      listHeader: Locale.tr("VC Templates"),
      infoHeader: Locale.tr("VC Template"),
      lockable: true,
      subheader: '<span>\
          <span class="total_templates"/> <small>'+Locale.tr("TOTAL")+'</small>\
        </span>',
      resource: 'VCTemplate',
      buttons: Buttons,
      actions: Actions,
      dataTable: new Table(DATATABLE_ID, {actions: true, info: true}),
    //   panels: _panels,
    //   panelsHooks: _panelsHooks,
      formPanels: _formPanels,
      dialogs: _dialogs
    };
  
    return Tab;
  });
  