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

#include "VirtualClusterPool.h"
#include "VirtualMachineHook.h"
#include "NebulaLog.h"
#include "NebulaUtil.h"
#include "Nebula.h"
#include "Snapshots.h"
#include "ScheduledAction.h"


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

const char * VirtualClusterPool::import_table = "vc_import";

const char * VirtualClusterPool::import_db_names = "deploy_id, vcid";

const char * VirtualClusterPool::import_db_bootstrap =
    "CREATE TABLE IF NOT EXISTS vc_import "
    "(deploy_id VARCHAR(128), vcid INTEGER, PRIMARY KEY(deploy_id))";

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

VirtualClusterPool::VirtualClusterPool(SqlDB *         db):
                PoolSQL(db, VirtualCluster::table)    
{
    // new VirtualClusterPool
}


int VirtualClusterPool::allocate (
    int            uid,
    int            gid,
    const string&  uname,
    const string&  gname,
    int            umask,
    int            vms_amount,
    string         nfs_location,
    VirtualMachineTemplate * vm_template,    
    int *          oid,
    string&        error_str,
    bool           on_hold)
{    
    VirtualCluster * vc;        
    VirtualMachinePool * vmpool;
    
    // ------------------------------------------------------------------------
    // Build a new Virtual Cluster object
    // ------------------------------------------------------------------------    
    vc = new VirtualCluster(-1,-1,"","",nfs_location,0,0);        

    vmpool = Nebula::instance().get_vmpool();

    vmpool->allocate_in_vc(uid, gid, uname, gname, umask,
            vm_template,* vc, oid, error_str, on_hold);                       
    // for(int i = 0; i < vms_amount; i++) 
    // {

    // }

    return *oid;
}


int VirtualClusterPool::get_running(
    vector<int>&    oids,
    int             vc_limit,
    time_t          last_poll)
{
    ostringstream   os;
    string          where;

    os << "last_poll <= " << last_poll << " and"
       << " state = " << VirtualCluster::ACTIVE
       << " and ( lcm_state = " << VirtualCluster::RUNNING << " )"
       << " ORDER BY last_poll ASC LIMIT " << vc_limit;

    where = os.str();

    return PoolSQL::search(oids,VirtualCluster::table,where);
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */