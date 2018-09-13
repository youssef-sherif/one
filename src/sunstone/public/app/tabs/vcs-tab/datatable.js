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
    /*
      DEPENDENCIES
     */
  
    var TabDataTable = require('utils/tab-datatable');
    var VCTableUtils = require('./utils/datatable-common');
    var OpenNebulaVC = require('opennebula/vc');
    var SunstoneConfig = require('sunstone-config');
    var Locale = require('utils/locale');
    var StateActions = require('./utils/state-actions');
    var Sunstone = require('sunstone');
    var Vnc = require('utils/vnc');
    var Spice = require('utils/spice');
    var Notifier = require('utils/notifier');
    var DashboardUtils = require('utils/dashboard');
    var SearchDropdown = require('hbs!./datatable/search');
  
    /*
      CONSTANTS
     */
  
    var RESOURCE = "VC";
    var XML_ROOT = "VC";
    var TAB_NAME = require('./tabId');
    var LABELS_COLUMN = 13;
    var SEARCH_COLUMN = 14;
  
  
    /*
      CONSTRUCTOR
     */
  
    function Table(dataTableId, conf) {
      this.conf = conf || {};
      this.tabId = TAB_NAME;
      this.dataTableId = dataTableId;
      this.resource = RESOURCE;
      this.xmlRoot = XML_ROOT;
      this.labelsColumn = LABELS_COLUMN;
  
      this.dataTableOptions = {
        "bAutoWidth": false,
        "bSortClasses" : false,
        "bDeferRender": true,
        "aoColumnDefs": [
            {"sType": "ip-address", "aTargets": [0]},
            {"sType": "num", "aTargets": [1]},
            {"sType": "date-euro", "aTargets": [ 10 ]},
            {"bSortable": false, "aTargets": ["check", 6, 7, 11]},
            {"sWidth": "35px", "aTargets": [0]},
            {"bVisible": true, "aTargets": SunstoneConfig.tabTableColumns(TAB_NAME)},
            {"bVisible": false, "aTargets": ['_all']}
        ]
      }
  
      this.columns = VCTableUtils.columns;
  
      this.selectOptions = {
        "id_index": 1,
        "name_index": 4,
        "select_resource": Locale.tr("Please select a VC from the list"),
        "you_selected": Locale.tr("You selected the following VC:"),
        "select_resource_multiple": Locale.tr("Please select one or more VCs from the list"),
        "you_selected_multiple": Locale.tr("You selected the following VCs:")
      };
  
      this.totalVcs = 0;
      this.activeVcs = 0;
      this.pendingVcs = 0;
      this.failedVcs = 0;
      this.offVcs = 0;
  
      this.conf.searchDropdownHTML = SearchDropdown({tableId: this.dataTableId});
      this.searchColumn = SEARCH_COLUMN;
  
      TabDataTable.call(this);
    };
  
    Table.prototype = Object.create(TabDataTable.prototype);
    Table.prototype.constructor = Table;
    Table.prototype.elementArray = _elementArray;
    Table.prototype.initialize = _initialize;
    Table.prototype.preUpdateView = _preUpdateView;
    Table.prototype.postUpdateView = _postUpdateView;
  
    return Table;
  
    /*
      FUNCTION DEFINITIONS
     */
  
    function _elementArray(element_json) {
      var element = element_json[XML_ROOT];
      var state = parseInt(element.STATE);
  
      this.totalVcs++;
      switch (state) {
        case OpenNebulaVC.STATES.INIT:
        case OpenNebulaVC.STATES.PENDING:
        case OpenNebulaVC.STATES.HOLD:
          this.pendingVcs++;
          break;
        case OpenNebulaVC.STATES.ACTIVE:
          if (OpenNebulaVC.isFailureState(element.LCM_STATE)) {
            this.failedVcs++;
          } else {
            this.activeVcs++;
          }
          break;
        case OpenNebulaVC.STATES.STOPPED:
        case OpenNebulaVC.STATES.SUSPENDED:
        case OpenNebulaVC.STATES.POWEROFF:
          this.offVcs++;
          break;
        default:
          break;
      }
  
      return VCsTableUtils.elementArray(element_json);
    }
  
    function _preUpdateView() {
      var tab = $('#' + TAB_NAME);
      if (!Sunstone.rightInfoVisible(tab)){
        StateActions.disableAllStateActions();
      }
  
      this.totalVcs = 0;
      this.activeVcs = 0;
      this.pendingVcs = 0;
      this.failedVcs = 0;
      this.offVcs = 0;
    }
  
    function _postUpdateView() {
      $(".total_vcs").removeClass("fadeinout");
      DashboardUtils.counterAnimation(".total_vcs", this.totalVcs);
  
      $(".active_vcs").removeClass("fadeinout");
      DashboardUtils.counterAnimation(".active_vcs", this.activeVcs);
  
      $(".pending_vcs").removeClass("fadeinout");
      DashboardUtils.counterAnimation(".pending_vcs", this.pendingVcs);
  
      $(".failed_vcs").removeClass("fadeinout");
      DashboardUtils.counterAnimation(".failed_vcs", this.failedVcs);
  
      $(".off_vcs").text(this.offVcs);
    }
  
    function _initialize(opts) {
      var that = this;
  
      TabDataTable.prototype.initialize.call(this, opts);
  
      $('#' + this.dataTableId).on("click", '.vnc', function() {
        var vmId = $(this).attr('vc_id');
  
        if (!Vnc.lockStatus()) {
          Vnc.lock();
          Sunstone.runAction("VC.startvnc_action", vmId);
        } else {
          Notifier.notifyError(Locale.tr("VNC Connection in progress"))
        }
  
        return false;
      });
  
      $('#' + this.dataTableId).on("click", '.spice', function() {
        var vmId = $(this).attr('vc_id');
  
        if (!Spice.lockStatus()) {
          Spice.lock();
          Sunstone.runAction("VC.startspice_action", vmId);
        } else {
          Notifier.notifyError(Locale.tr("SPICE Connection in progress"))
        }
  
        return false;
      });
  
      $('#' + this.dataTableId).on("change", 'tbody input.check_item', function() {
        if ($(this).is(":checked")){
          StateActions.enableStateActions($(this).attr("state"), $(this).attr("lcm_state"));
        } else {
          // First disable all actions
          StateActions.disableAllStateActions();
  
          // Enable actions available to any of the selected VMs
          var nodes = $('tr', that.dataTable); //visible nodes only
          $.each($('input.check_item:checked', nodes), function(){
            StateActions.enableStateActions($(this).attr("state"), $(this).attr("lcm_state"));
          });
  
        }
  
        return true;
      });
  
    }
  
  
  });
  