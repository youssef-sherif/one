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
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <queue>

#include "VirtualMachine.h"
#include "VirtualCluster.h"
#include "VirtualNetworkPool.h"
#include "ImagePool.h"
#include "NebulaLog.h"
#include "NebulaUtil.h"
#include "Snapshots.h"
#include "ScheduledAction.h"

#include "Nebula.h"

/* ************************************************************************** */
/* Virtual Cluster :: Database Access Functions                               */
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
                               const string  _nfs_location,                                                                      
                               int           umask,                               
                               VirtualMachineTemplate * _vm_template):
                        PoolObjectSQL(-1,VC,"",_uid,_gid,_uname,_gname,table),               
                        state(INIT),
                        prev_state(INIT),
                        lcm_state(LCM_INIT),
                        prev_lcm_state(LCM_INIT),
                        resched(0),
                        stime(time(0)),
                        etime(0),
                        deploy_id(""),
                        _log(0)                        
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

    slave_vms = new vector<VirtualMachine *>();

    set_umask(umask);    
}


VirtualCluster::~VirtualCluster()
{
    delete user_obj_template;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VirtualCluster::vc_state_from_str(string& st, VcState& state)
{
    one_util::toupper(st);

    if ( st == "INIT" ) {
        state = INIT;
    } else if ( st == "PENDING" ) {
		state = PENDING;
    } else if ( st == "HOLD" ) {
		state = HOLD;
    } else if ( st == "ACTIVE" ) {
		state = ACTIVE;
    } else if ( st == "SUSPENDED" ) {
		state = SUSPENDED;
    } else if ( st == "DONE" ) {
		state = DONE;
    } else {
        return -1;
    }

    return 0;
}

/* ------------------------------------------------------------------------ */
/* -------------------------------------------------------------------------- */

string& VirtualCluster::vc_state_to_str(string& st, VcState state)
{
    switch (state)
    {
        case INIT:
			st = "INIT"; break;
        case PENDING:
			st = "PENDING"; break;
        case HOLD:
			st = "HOLD"; break;
        case ACTIVE:
			st = "ACTIVE"; break;
        case SUSPENDED:
			st = "SUSPENDED"; break;
        case DONE:
			st = "DONE"; break;
    }

    return st;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VirtualCluster::lcm_state_from_str(string& st, LcmState& state)
{
    one_util::toupper(st);

    if ( st == "LCM_INIT" ){
        state = LCM_INIT;
    } else if ( st == "PROLOG") {
        state = PROLOG;
    } else if ( st == "BOOT") {
        state = BOOT;
    } else if ( st == "RUNNING") {
        state = RUNNING;
    } else {
        return -1;
    }

    return 0;
}

/* -------------------------------------------------------------------------- */

string& VirtualCluster::lcm_state_to_str(string& st, LcmState state)
{
    switch (state)
    {
        case LCM_INIT:
            st = "LCM_INIT"; break;
        case PROLOG:
            st = "PROLOG"; break;
        case BOOT:
            st = "BOOT"; break;
        case RUNNING:
            st = "RUNNING"; break;            			
    }

    return st;
}

/* -------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------ */

int VirtualCluster::bootstrap(SqlDB * db)
{
    int rc;

    ostringstream oss_vm(VirtualCluster::db_bootstrap);

    ostringstream oss_index("CREATE INDEX state_idx on vc_pool (state);");

    rc =  db->exec_local_wr(oss_vm);
    rc += db->exec_local_wr(oss_index);

    return rc;
};

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */

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


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

string& VirtualCluster::to_xml_extended(string& xml, int n_history) const
{
    string template_xml;
    string user_template_xml;        
    string perm_xml;
    string snap_xml;
    string lock_str;

    ostringstream   oss;

    oss << "<VC>"
        << "<ID>"        << oid       << "</ID>"
        << "<UID>"       << uid       << "</UID>"
        << "<GID>"       << gid       << "</GID>"
        << "<UNAME>"     << uname     << "</UNAME>"
        << "<GNAME>"     << gname     << "</GNAME>"
        << "<NAME>"      << name      << "</NAME>"
        << "<VMS_AMT>"   << vms_amount << "</VMS_AMT>"
        << "<NFS_LOC>"   << nfs_location << "</NFS_LOC>"        
        << "<STIME>"     << stime     << "</STIME>"
        << lock_db_to_xml(lock_str)        
        << obj_template->to_xml(template_xml)
        << user_obj_template->to_xml(user_template_xml);

    oss << "</VC>";

    xml = oss.str();

    return xml;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VirtualCluster::from_xml(const string &xml_str)
{
    vector<xmlNodePtr> content;

    int istate;
    int ilcmstate;
    int rc = 0;

    // Initialize the internal XML object
    rc = update_from_str(xml_str);

    if ( rc != 0 )
    {
        return -1;
    }

    // Get class base attributes
    rc += xpath(oid,       "/VM/ID",    -1);

    rc += xpath(uid,       "/VM/UID",   -1);
    rc += xpath(gid,       "/VM/GID",   -1);

    rc += xpath(uname,     "/VM/UNAME", "not_found");
    rc += xpath(gname,     "/VM/GNAME", "not_found");
    rc += xpath(name,      "/VM/NAME",  "not_found");
    
    rc += xpath(resched, "/VM/RESCHED", 0);

    rc += xpath<time_t>(stime, "/VM/STIME", 0);
    rc += xpath<time_t>(etime, "/VM/ETIME", 0);
    rc += xpath(deploy_id, "/VM/DEPLOY_ID","");

    // Permissions
    rc += perms_from_xml();

    //VM states
    rc += xpath(istate,    "/VM/STATE",     0);
    rc += xpath(ilcmstate, "/VM/LCM_STATE", 0);

    state     = static_cast<VcState>(istate);
    lcm_state = static_cast<LcmState>(ilcmstate);

    xpath(istate,    "/VM/PREV_STATE",     istate);
    xpath(ilcmstate, "/VM/PREV_LCM_STATE", ilcmstate);

    prev_state     = static_cast<VcState>(istate);
    prev_lcm_state = static_cast<LcmState>(ilcmstate);

    rc += lock_db_from_xml();

    // -------------------------------------------------------------------------
    // Virtual Machine template and attributes
    // -------------------------------------------------------------------------
    ObjectXML::get_nodes("/VM/TEMPLATE", content);

    if (content.empty())
    {
        return -1;
    }
    rc += obj_template->from_xml_node(content[0]);


    // -------------------------------------------------------------------------
    // Virtual Machine user template
    // -------------------------------------------------------------------------
    ObjectXML::get_nodes("/VM/USER_TEMPLATE", content);

    if (content.empty())
    {
        return -1;
    }

    rc += user_obj_template->from_xml_node(content[0]);

    ObjectXML::free_nodes(content);
    content.clear();
    
    if (rc != 0)
    {
        return -1;
    }

    return 0;
}

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */

int VirtualCluster::insert_replace(SqlDB *db, bool replace, string& error_str)
{
    ostringstream   oss;
    int             rc;

    string xml_body;
    char * sql_name;
    char * sql_xml;

    sql_name =  db->escape_str(name.c_str());

    if ( sql_name == 0 )
    {
        goto error_generic;
    }

    sql_xml = db->escape_str(to_xml(xml_body).c_str());

    if ( sql_xml == 0 )
    {
        goto error_body;
    }

    if ( validate_xml(sql_xml) != 0 )
    {
        goto error_xml;
    }

    if(replace)
    {
        oss << "REPLACE";
    }
    else
    {
        oss << "INSERT";
    }

    oss << " INTO " << table << " ("<< db_names <<") VALUES ("
        <<          oid             << ","
        << "'" <<   sql_name        << "',"
        << "'" <<   sql_xml         << "',"
        <<          uid             << ","
        <<          gid             << ","        
        <<          state           << ","
        <<          lcm_state       << ","
        <<          owner_u         << ","
        <<          group_u         << ","
        <<          other_u         << ")";

    db->free_str(sql_name);
    db->free_str(sql_xml);

    rc = db->exec_wr(oss);

    return rc;

error_xml:
    db->free_str(sql_name);
    db->free_str(sql_xml);

    error_str = "Error transforming the VC to XML.";

    goto error_common;

error_body:
    db->free_str(sql_name);
    goto error_generic;

error_generic:
    error_str = "Error inserting VC in DB.";
error_common:
    return -1;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int VirtualCluster::insert(SqlDB * db, string& error_str)
{
    int    rc;
    string name;
    string prefix;

    string value;
    long int ivalue;
    float fvalue;
    set<int> cluster_ids;
    vector<Template *> quotas;

    ostringstream oss;

    // ------------------------------------------------------------------------
    // Set a name if the VM has not got one and VM_ID
    // ------------------------------------------------------------------------
    user_obj_template->erase("VMID");
    obj_template->add("VMID", oid);

    user_obj_template->get("TEMPLATE_ID", value);
    user_obj_template->erase("TEMPLATE_ID");

    if (!value.empty())
    {
        obj_template->add("TEMPLATE_ID", value);
    }

    user_obj_template->get("NAME",name);
    user_obj_template->erase("NAME");

    user_obj_template->get("TEMPLATE_NAME", prefix);
    user_obj_template->erase("TEMPLATE_NAME");

    if (prefix.empty())
    {
        prefix = "one";
    }

    if (name.empty() == true)
    {
        oss.str("");
        oss << prefix << "-" << oid;
        name = oss.str();
    }

    if ( !PoolObjectSQL::name_is_valid(name, error_str) )
    {
        goto error_name;
    }

    this->name = name;

    // ------------------------------------------------------------------------
    // Check for EMULATOR attribute
    // ------------------------------------------------------------------------

    user_obj_template->get("EMULATOR", value);

    if (!value.empty())
    {
        user_obj_template->erase("EMULATOR");
        obj_template->add("EMULATOR", value);
    }

    // ------------------------------------------------------------------------
    // Check for CPU, VCPU and MEMORY attributes
    // ------------------------------------------------------------------------

    if ( user_obj_template->get("MEMORY", ivalue) == false || (ivalue * 1024) <= 0 )
    {
        goto error_memory;
    }

    user_obj_template->erase("MEMORY");
    obj_template->add("MEMORY", ivalue);

    if ( user_obj_template->get("CPU", fvalue) == false || fvalue <= 0 )
    {
        goto error_cpu;
    }

    user_obj_template->erase("CPU");
    obj_template->add("CPU", fvalue);

    // VCPU is optional, first check if the attribute exists, then check it is
    // an integer
    user_obj_template->get("VCPU", value);

    if ( value.empty() == false )
    {
        if ( user_obj_template->get("VCPU", ivalue) == false || ivalue <= 0 )
        {
            goto error_vcpu;
        }

        user_obj_template->erase("VCPU");
        obj_template->add("VCPU", ivalue);
    }

    // ------------------------------------------------------------------------
    // Check the cost attributes
    // ------------------------------------------------------------------------

    if ( user_obj_template->get("CPU_COST", fvalue) == true )
    {
        if ( fvalue < 0 )
        {
            goto error_cpu_cost;
        }

        user_obj_template->erase("CPU_COST");
        obj_template->add("CPU_COST", fvalue);
    }

    if ( user_obj_template->get("MEMORY_COST", fvalue) == true )
    {
        if ( fvalue < 0 )
        {
            goto error_memory_cost;
        }

        user_obj_template->erase("MEMORY_COST");
        obj_template->add("MEMORY_COST", fvalue);
    }

    if ( user_obj_template->get("DISK_COST", fvalue) == true )
    {
        if ( fvalue < 0 )
        {
            goto error_disk_cost;
        }

        user_obj_template->erase("DISK_COST");
        obj_template->add("DISK_COST", fvalue);
    }

    // ------------------------------------------------------------------------
    // Get network leases
    // ------------------------------------------------------------------------

    // rc = get_network_leases(error_str);

    // if ( rc != 0 )
    // {
    //     goto error_leases_rollback;
    // }


    bool on_hold;

    if (user_obj_template->get("SUBMIT_ON_HOLD", on_hold) == true)
    {
        user_obj_template->erase("SUBMIT_ON_HOLD");

        obj_template->replace("SUBMIT_ON_HOLD", on_hold);
    }

    // ------------------------------------------------------------------------
    // Insert the VC
    // ------------------------------------------------------------------------

    rc = insert_replace(db, false, error_str);


    return 0;

    // -------------------------------------------------------------------------
    // Get and set DEPLOY_ID for imported VMs
    // -------------------------------------------------------------------------

    user_obj_template->get("IMPORT_VM_ID", value);
    user_obj_template->erase("IMPORT_VM_ID");

    if (!value.empty())
    {
        const char * one_vms = "^one-[[:digit:]]+$";

        if (one_util::regex_match(one_vms, value.c_str()) == 0)
        {
            goto error_one_vms;
        }
        else
        {
            deploy_id = value;
            obj_template->add("IMPORTED", "YES");
        }
    }

    // ------------------------------------------------------------------------
    // Insert the VM
    // ------------------------------------------------------------------------

    rc = insert_replace(db, false, error_str);

    if ( rc != 0 )
    {
        goto error_update;
    }

    //-------------------------------------------------------------------------
    //Check for missing unlock callbacks during creation
    //-------------------------------------------------------------------------

    // if ( state == VirtualMachine::CLONING )
    // {
    //     Nebula::instance().get_lcm()->trigger(LCMAction::DISK_LOCK_SUCCESS,oid);
    // }

    return 0;

error_update:
    goto error_rollback;

error_boot_order:
    goto error_rollback;

error_context:
    goto error_rollback;

error_requirements:
    goto error_rollback;

error_graphics:
    goto error_rollback;

error_rollback:    

error_leases_rollback:    
    goto error_common;

error_cpu:
    error_str = "CPU attribute must be a positive float or integer value.";
    goto error_common;

error_vcpu:
    error_str = "VCPU attribute must be a positive integer value.";
    goto error_common;

error_memory:
    error_str = "MEMORY attribute must be a positive integer value.";
    goto error_common;

error_cpu_cost:
    error_str = "CPU_COST attribute must be a positive float or integer value.";
    goto error_common;

error_memory_cost:
    error_str = "MEMORY_COST attribute must be a positive float or integer value.";
    goto error_common;

error_disk_cost:
    error_str = "DISK_COST attribute must be a positive float or integer value.";
    goto error_common;

error_one_vms:
    error_str = "Trying to import an OpenNebula VM: 'one-*'.";
    goto error_common;

error_os:
error_pci:
error_defaults:
error_vrouter:
error_public:
error_name:
error_common:
    NebulaLog::log("ONE",Log::ERROR, error_str);

    return -1;
}

/* -------------------------------------------------------------------------- */