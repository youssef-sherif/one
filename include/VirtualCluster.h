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

#include "VirtualMachineTemplate.h"
#include "PoolSQL.h"
#include "History.h"
#include "Image.h"
#include "Log.h"
#include "NebulaLog.h"
#include "NebulaUtil.h"
#include "Quotas.h"

#include <time.h>
#include <set>
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
        return new VirtualMachineTemplate;
    }

    // /**
    //  *  Fills a auth class to perform an authZ/authN request based on the object
    //  *  attributes. Disables the cluster and all NET rules (NET* and NET/%) for
    //  *  reservations.
    //  *    @param auths to be filled
    //  */
    // void get_permissions(PoolObjectAuth& auths);


    // *************************************************************************
    // Virtual Cluster management methods
    // ************************************************************************

    /*
    *  return the number of Virtual Machines in Virtual Cluster
    */
    int get_vms_amount()
    {
        return vms_amount;
    }  

    /*
    *  Getter that returns master Virtual Machine
    */
    VirtualMachine * get_master()
    {
        return master_vm;
    }

    /*
    *  Function to add a slave to Virtual Cluster
    */
    void add_vm(VirtualMachine & vm);

    /*
    *  Function to remove a slave from Virtual Cluster  
    */
    void remove_vm(VirtualMachine & vm);
    
    
    // *************************************************************************


    // /**
    //  * Function to print the VirtualCluster object into a string in
    //  * XML format
    //  *  @param xml the resulting XML string
    //  *  @return a reference to the generated string
    //  */
    string& to_xml(string& xml) const
    {
        return to_xml_extended(xml, 1);
    }


        /**
     * Function to print the VirtualMachine object into a string in
     * XML format, with extended information (full history records)
     *  @param xml the resulting XML string
     *  @return a reference to the generated string
     */
    string& to_xml_extended(string& xml) const
    {
        return to_xml_extended(xml, 2);
    }

    /**
     *  Rebuilds the object from an xml formatted string
     *    @param xml_str The xml-formatted string
     *
     *    @return 0 on success, -1 otherwise
     */
    int from_xml(const string &xml_str);

    // /**
    //  *  Gets a string based attribute (single) from an address range. If the
    //  *  attribute is not found in the address range, the VNET template will be
    //  *  used
    //  *    @param name of the attribute
    //  *    @param value of the attribute (a string), will be "" if not defined or
    //  *    not a single attribute
    //  *    @param ar_id of the address attribute.
    //  */
    // void get_template_attribute(const char * name, string& value, int ar_id) const;

    // /**
    //  *  int version of get_template_attribute
    //  *    @return 0 on success
    //  */
    // int get_template_attribute(const char * name, int& value, int ar_id) const;

    // /**
    //  *  Adds the security group of the VNet and its ARs to the given set
    //  *    @param sgs to put the sg ids in
    //  */
    // void get_security_groups(set<int> & sgs);

    /**
     *    @return A copy of the VNET Template
     */
    VirtualMachineTemplate * clone_template() const
    {
        VirtualMachineTemplate * new_vc = new VirtualMachineTemplate(
                *(static_cast<VirtualMachineTemplate *>(obj_template)));

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
     *  Number of Virtual Machines in the VC
     */
    int vms_amount;

    /**
     *  Complete set of slave Virtual Machines records for the VC
     */
    vector<VirtualMachine * > * slave_vms;

    /**
     *  master Virtual Machine
     */
    VirtualMachine * master_vm;

    /**
     *  Security Groups
     */
    set<int> security_groups;

    /**
     *  User template to store custom metadata. This template can be updated
     */
    VirtualMachineTemplate * user_obj_template;

    /**
     *  Deployment specific identification string, as returned by the VC driver
     */
    string      deploy_id;

    /**
     *  The state of the virtual cluster.
     */
    VcState     state;

    /**
     *  Previous state og the virtual cluster, to trigger state hooks
     */
    VcState     prev_state;

    /**
     *  The state of the virtual cluster (in the Life-cycle Manager).
     */
    LcmState    lcm_state;

    /**
     *  Previous state og the virtual cluster, to trigger state hooks
     */
    LcmState    prev_lcm_state;

    /**
     *  Marks the VC as to be re-scheduled
     */
    int         resched;

    /**
     *  Start time, the VC enter the nebula system (in epoch)
     */
    time_t      stime;

    /**
     *  Exit time, the VC leave the nebula system (in epoch)
     */
    time_t      etime;

    /**
     *  Log class for the virtual cluster, it writes log messages in
     *          $ONE_LOCATION/var/$VID/vm.log
     *  or, in case that OpenNebula is installed in root
     *          /var/log/one/$VM_ID.log
     *  For the syslog it will use the predefined /var/log/ locations
     */
    Log *       _log;


    //**************************************************************************
    // Constructor
    //**************************************************************************

    VirtualCluster( int                     uid,
                   int                      gid,
                   const string&            _uname,
                   const string&            _gname,
                   const string             _nfs_location,                   
                   int                      umask,                   
                   VirtualMachineTemplate * _vm_template = 0);

    ~VirtualCluster();

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
     *  Function that renders the VM in XML format optinally including
     *  extended information (all history records)
     *  @param xml the resulting XML string
     *  @param n_history Number of history records to include:
     *      0: none
     *      1: the last one
     *      2: all
     *  @return a reference to the generated string
     */
    string& to_xml_extended(string& xml, int n_history) const;
    /**
     *  Bootstraps the database table(s) associated to the Virtual Cluster
     *    @return 0 on success
     */

    /**
     * Inserts the last monitoring, and deletes old monitoring entries.
     *
     * @param db pointer to the db
     * @return 0 on success
     */
    // int update_monitoring(SqlDB * db);


    static int bootstrap(SqlDB * db);

    // /**
    //  *  Rebuilds the object from an xml formatted string
    //  *    @param xml_str The xml-formatted string
    //  *
    //  *    @return 0 on success, -1 otherwise
    //  */
    // int from_xml(const string &xml_str);

    /**
     * Updates the BRIDGE, PHYDEV, and VLAN_ID attributes.
     *    @param error string describing the error if any
     *    @return 0 on success
     */
    // int post_update_template(string& error);

    // *************************************************************************
    // DataBase implementation
    // *************************************************************************

    static const char * table;

    static const char * db_names;

    static const char * db_bootstrap;

    /**
     *  Writes the Virtual Cluster and its associated template and leases in the database.
     *    @param db pointer to the db
    //  *    @return 0 on success
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
