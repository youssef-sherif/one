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
#include <string.h>

#include <iostream>
#include <sstream>
#include <queue>

#include "VirtualCluster.h"
#include "VirtualMachinePool.h"
#include "VirtualClusterPool.h"
#include "NebulaLog.h"
#include "NebulaUtil.h"
#include "Snapshots.h"
#include "ScheduledAction.h"

#include "Nebula.h"

/* ************************************************************************** */
/* Virtual Network :: Database Access Functions                               */
/* ************************************************************************** */

const char * VirtualCluster::table    = "vc_pool";

const char * VirtualCluster::db_names =
        "oid, name, body, uid, gid, owner_u, group_u, other_u, pid";

const char * VirtualCluster::db_bootstrap = "CREATE TABLE IF NOT EXISTS"
    " vc_pool (oid INTEGER PRIMARY KEY, name VARCHAR(128),"
    " body MEDIUMTEXT, uid INTEGER, gid INTEGER,"
    " owner_u INTEGER, group_u INTEGER, other_u INTEGER,"
    " pid INTEGER, UNIQUE(name,uid))";


VirtualCluster::VirtualCluster(int           _uid,
                               int           _gid,
                               const string& _uname,
                               const string& _gname,
                               const string& _nfs_location,                                                                      
                               int           umask,                               
                               VirtualMachineTemplate * _vm_template):                               
        PoolObjectSQL(-1,VC,"",_uid,_gid,_uname,_gname,table)
{

    if (_vm_template != 0)
    {
        // This is a VM Template, with the root TEMPLATE.
        _vm_template->set_xml_root("USER_TEMPLATE");

        user_obj_template = _vm_template;
    }
    else
    {
        user_obj_template = new VirtualMachineTemplate(false,'=',"USER_TEMPLATE");
    }

    obj_template = new VirtualMachineTemplate;

    set_umask(umask);

}


VirtualCluster::~VirtualCluster()
{

}


void VirtualCluster::add_vm(VirtualMachine & vm) 
{
    if(slave_vms->size() == 0) 
    {
        master_vm = & vm;
        vms_amount++;
        return;
    }
    slave_vms->push_back( & vm);
    vms_amount++;
}


void VirtualCluster::remove_vm(VirtualMachine & vm) 
{        
    vector<VirtualMachine *>::iterator position = std::find(slave_vms->begin(), slave_vms->end(), & vm);        
    if (position != slave_vms->end()) 
    {
        slave_vms->erase(position);            
        vms_amount--;
    }        
}



/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
