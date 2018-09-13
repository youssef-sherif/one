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

#ifndef VIRTUAL_CLUSTER_H_
#define VIRTUAL_CLUSTER_H_


#include "PoolSQL.h"
#include "VirtualClusterTemplate.h"
#include "VirtualMachine.h"

#include <vector>
#include <string>
#include <map>

#include <time.h>
#include <sstream>

using namespace std;


/**
 *  The Virtual Cluster class. It represents a VC...
 */
class VirtualCluster : public PoolObjectSQL
{
public:
    // -------------------------------------------------------------------------
    // VC States
    // -------------------------------------------------------------------------
    /**
     *  Global Virtual Machine state
     */
    enum VcState
    {
        INIT            = 0,
        PENDING         = 1,
        HOLD            = 2,
        ACTIVE          = 3,
        STOPPED         = 4,
        SUSPENDED       = 5,
        DONE            = 6,
        //FAILED        = 7,
        POWEROFF        = 8,
        UNDEPLOYED      = 9,
        CLONING         = 10,
        CLONING_FAILURE = 11
    };

    /**
     *  Virtual Machine state associated to the Life-cycle Manager
     */
    enum LcmState
    {
        LCM_INIT            = 0,
        PROLOG              = 1,
        BOOT                = 2,
        RUNNING             = 3,
        MIGRATE             = 4,
        SAVE_STOP           = 5,
        SAVE_SUSPEND        = 6,
        SAVE_MIGRATE        = 7,
        PROLOG_MIGRATE      = 8,
        PROLOG_RESUME       = 9,
        EPILOG_STOP         = 10,
        EPILOG              = 11,
        SHUTDOWN            = 12,
        //CANCEL            = 13,
        //FAILURE           = 14,
        CLEANUP_RESUBMIT    = 15,
        UNKNOWN             = 16,
        HOTPLUG             = 17,
        SHUTDOWN_POWEROFF   = 18,
        BOOT_UNKNOWN        = 19,
        BOOT_POWEROFF       = 20,
        BOOT_SUSPENDED      = 21,
        BOOT_STOPPED        = 22,
        CLEANUP_DELETE      = 23,
        HOTPLUG_SNAPSHOT    = 24,
        HOTPLUG_NIC         = 25,
        HOTPLUG_SAVEAS           = 26,
        HOTPLUG_SAVEAS_POWEROFF  = 27,
        HOTPLUG_SAVEAS_SUSPENDED = 28,
        SHUTDOWN_UNDEPLOY   = 29,
        EPILOG_UNDEPLOY     = 30,
        PROLOG_UNDEPLOY     = 31,
        BOOT_UNDEPLOY       = 32,
        HOTPLUG_PROLOG_POWEROFF = 33,
        HOTPLUG_EPILOG_POWEROFF = 34,
        BOOT_MIGRATE            = 35,
        BOOT_FAILURE            = 36,
        BOOT_MIGRATE_FAILURE    = 37,
        PROLOG_MIGRATE_FAILURE  = 38,
        PROLOG_FAILURE          = 39,
        EPILOG_FAILURE          = 40,
        EPILOG_STOP_FAILURE     = 41,
        EPILOG_UNDEPLOY_FAILURE = 42,
        PROLOG_MIGRATE_POWEROFF = 43,
        PROLOG_MIGRATE_POWEROFF_FAILURE = 44,
        PROLOG_MIGRATE_SUSPEND          = 45,
        PROLOG_MIGRATE_SUSPEND_FAILURE  = 46,
        BOOT_UNDEPLOY_FAILURE   = 47,
        BOOT_STOPPED_FAILURE    = 48,
        PROLOG_RESUME_FAILURE   = 49,
        PROLOG_UNDEPLOY_FAILURE = 50,
        DISK_SNAPSHOT_POWEROFF         = 51,
        DISK_SNAPSHOT_REVERT_POWEROFF  = 52,
        DISK_SNAPSHOT_DELETE_POWEROFF  = 53,
        DISK_SNAPSHOT_SUSPENDED        = 54,
        DISK_SNAPSHOT_REVERT_SUSPENDED = 55,
        DISK_SNAPSHOT_DELETE_SUSPENDED = 56,
        DISK_SNAPSHOT        = 57,
        //DISK_SNAPSHOT_REVERT = 58,
        DISK_SNAPSHOT_DELETE = 59,
        PROLOG_MIGRATE_UNKNOWN = 60,
        PROLOG_MIGRATE_UNKNOWN_FAILURE = 61,
        DISK_RESIZE = 62,
        DISK_RESIZE_POWEROFF = 63,
        DISK_RESIZE_UNDEPLOYED = 64
    };

    /**
     *  Functions to convert to/from string the VC states
     */
    static int vc_state_from_str(string& st, VcState& state);

    static string& vc_state_to_str(string& st, VcState state);

    static int lcm_state_from_str(string& st, LcmState& state);

    static string& lcm_state_to_str(string& st, LcmState state);

    // *************************************************************************
    // Virtual Cluster Public Methods
    // *************************************************************************

    /**
     *  Factory method for virtual cluster templates
     */
    Template * get_new_template() const
    {
        return new VirtualClusterTemplate;
    }

    /**
     *  Fills a auth class to perform an authZ/authN request based on the object
     *  attributes. Disables the cluster and all NET rules (NET* and NET/%) for
     *  reservations.
     *    @param auths to be filled
     */
    void get_permissions(PoolObjectAuth& auths);


    // *************************************************************************
    // Virtual Machine management methods
    // *************************************************************************

    void add_virtual_machine(VirtualMachine& vm);


    void remove_virtual_machine(VirtualMachine& vm);
    
    
    // *************************************************************************


    /**
     * Function to print the VirtualCluster object into a string in
     * XML format
     *  @param xml the resulting XML string
     *  @return a reference to the generated string
     */
    string& to_xml(string& xml) const;

    /**
     *  Gets a string based attribute (single) from an address range. If the
     *  attribute is not found in the address range, the VNET template will be
     *  used
     *    @param name of the attribute
     *    @param value of the attribute (a string), will be "" if not defined or
     *    not a single attribute
     *    @param ar_id of the address attribute.
     */
    void get_template_attribute(const char * name, string& value, int ar_id) const;

    /**
     *  int version of get_template_attribute
     *    @return 0 on success
     */
    int get_template_attribute(const char * name, int& value, int ar_id) const;

    /**
     *  Adds the security group of the VNet and its ARs to the given set
     *    @param sgs to put the sg ids in
     */
    void get_security_groups(set<int> & sgs);

    /**
     *    @return A copy of the VNET Template
     */
    VirtualClusterTemplate * clone_template() const
    {
        VirtualClusterTemplate * new_vc = new VirtualClusterTemplate(
                *(static_cast<VirtualClusterTemplate *>(obj_template)));

        return new_vc;
    };

private:

    // -------------------------------------------------------------------------
    // Friends
    // -------------------------------------------------------------------------
    friend class VirtualClusterPool;

    // *************************************************************************
    // Virtual Cluster Private Attributes
    // *************************************************************************

        /**
     *  Location of shared file system
     */
    string nfs_location;

    /**
     * Number of Virtual Machines in the VC
     */
    int vms_amount;

    /**
     *  Complete set of Virtual Machines records for the VC
     */
    vector<VirtualMachine*> vms;

    /**
     * Name of the vc mad
     */
    string vc_mad;


    /**
     *  Security Groups
     */
    set<int> security_groups;


    // *************************************************************************
    // DataBase implementation (Private)
    // *************************************************************************

    /**
     *  Execute an INSERT or REPLACE Sql query.
     *    @param db The SQL DB
     *    @param replace Execute an INSERT or a REPLACE
     *    @param error_str Returns the error reason, if any
     *    @return 0 on success
     */
    int insert_replace(SqlDB *db, bool replace, string& error_str);

    /**
     *  Bootstraps the database table(s) associated to the Virtual Cluster
     *    @return 0 on success
     */
    static int bootstrap(SqlDB * db);

    /**
     *  Rebuilds the object from an xml formatted string
     *    @param xml_str The xml-formatted string
     *
     *    @return 0 on success, -1 otherwise
     */
    int from_xml(const string &xml_str);

    /**
     * Updates the BRIDGE, PHYDEV, and VLAN_ID attributes.
     *    @param error string describing the error if any
     *    @return 0 on success
     */
    int post_update_template(string& error);

    //**************************************************************************
    // Constructor
    //**************************************************************************

    VirtualCluster( int                     uid,
                   int                      gid,
                   const string&            _uname,
                   const string&            _gname,
                   const string&            _nfs_location,
                   int                      _vms_amount,                
                   int                      _umask,                   
                   VirtualClusterTemplate * _vc_template = 0);

    ~VirtualCluster();

    // *************************************************************************
    // DataBase implementation
    // *************************************************************************

    static const char * table;

    static const char * db_names;

    static const char * db_bootstrap;

    /**
     *  Writes the Virtual Cluster and its associated template and leases in the database.
     *    @param db pointer to the db
     *    @return 0 on success
     */
    int insert(SqlDB * db, string& error_str);

    /**
     *  Writes/updates the Virtual Cluster data fields in the database.
     *    @param db pointer to the db
     *    @return 0 on success
     */
    int update(SqlDB * db)
    {
        string error_str;
        return insert_replace(db, true, error_str);
    }
};

#endif /*VIRTUAL_CLUSTER_H_*/
