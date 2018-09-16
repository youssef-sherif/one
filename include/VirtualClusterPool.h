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

#ifndef VIRTUAL_CLUSTER_POOL_H_
#define VIRTUAL_CLUSTER_POOL_H_

#include "PoolSQL.h"
#include "VirtualMachinePool.h"
#include "VirtualCluster.h"

#include <time.h>

using namespace std;

/**
 *  The Virtual Machine Pool class. ...
 */
class VirtualClusterPool : public PoolSQL
{
public:

    VirtualClusterPool(SqlDB *                      db);

    ~VirtualClusterPool(){};

    /**
     *  Function to allocate a new VC object
     *    @param uid user id (the owner of the VC)
     *    @param gid the id of the group this object is assigned to
     *    @param uname user name
     *    @param gname group name
     *    @param umask permissions umask
     *    @param vc_template a VC Template object describing the VC
     *    @param oid the id assigned to the VC (output)
     *    @param error_str Returns the error reason, if any
     *    @param on_hold flag to submit on hold
     *
     *    @return oid on success, -1 error inserting in DB or -2 error parsing
     *  the template
     */
    int allocate (
        int                      uid,
        int                      gid,
        const string&            uname,
        const string&            gname,
        int                      umask,
        int                      vms_amount,
        VirtualMachineTemplate * vm_template,
        int *                    oid,
        string&                  error_str,
        bool                     on_hold = false);

    /**
     *  Function to get a VC from the pool, if the object is not in memory
     *  it is loade from the DB
     *    @param oid VC unique id
     *    @param lock locks the VC mutex
     *    @return a pointer to the VC, 0 if the VC could not be loaded
     */
    VirtualCluster * get(
        int     oid)
    {
        return static_cast<VirtualCluster *>(PoolSQL::get(oid));
    };

    /**
     *  Function to get a VC from the pool, string version for VC ID
     */
    VirtualCluster * get(
        const string& oid_s)
    {
        istringstream iss(oid_s);
        int           oid;

        iss >> oid;

        if ( iss.fail() )
        {
            return 0;
        }

        return static_cast<VirtualCluster *>(PoolSQL::get(oid));
    };

    /**
     *  Gets a VC ID by its deploy_id, the dedploy_id - VC id mapping is keep
     *  in the import_table.
     *    @param deploy_id to search the id for
     *    @return -1 if not found or VCID
     *
     */
    int get_vcid(const string& deploy_id);

    /**
     *  Function to get the IDs of running VCs
     *   @param oids a vector that contains the IDs
     *   @param vc_limit Max. number of VCs returned
     *   @param last_poll Return only VCs which last_poll is less than or equal
     *          to this value.
     *   @return 0 on success
     */
    int get_running(
        vector<int>&    oids,
        int             vc_limit,
        time_t          last_poll);

    /**
     *  Function to get the IDs of pending VCs
     *   @param oids a vector that contains the IDs
     *   @return 0 on success
     */
    int get_pending(
        vector<int>&    oids);

    /**
     *  Gets the IDs of VCs matching the given SQL where string.
     *    @param oids a vector that contains the IDs
     *    @param where SQL clause
     *    @return 0 on success
     */
    int search(vector<int>& oids, const string& where)
    {
        return PoolSQL::search(oids, VirtualCluster::table, where);
    };

    /**
     * Deletes the expired monitoring entries for all VCs
     *
     * @return 0 on success
     */
    int clean_expired_monitoring();

    /**
     * Deletes all monitoring entries for all VCs
     *
     * @return 0 on success
     */
    int clean_all_monitoring();

    /**
     *  Bootstraps the database table(s) associated to the VirtualMachine pool
     *    @return 0 on success
     */
    static int bootstrap(SqlDB * _db)
    {
        int rc;
        ostringstream oss_import(import_db_bootstrap);

        rc  = VirtualCluster::bootstrap(_db);
        rc += _db->exec_local_wr(oss_import);

        return rc;
    };

    /**
     *  Dumps the VC pool in XML format. A filter can be also added to the query
     *  Also the hostname where the VirtualMachine is running is added to the
     *  pool
     *  @param oss the output stream to dump the pool contents
     *  @param where filter for the objects, defaults to all
     *  @param limit parameters used for pagination
     *  @param desc descending order of pool elements
     *
     *  @return 0 on success
     */
    int dump(ostringstream& oss, const string& where, const string& limit,
            bool desc)
    {
        return PoolSQL::dump(oss, "VC_POOL", VirtualCluster::table, where,
                             limit, desc);
    };

    /**
     *  Dumps the VC accounting information in XML format. A filter can be also
     *  added to the query as well as a time frame.
     *  @param oss the output stream to dump the pool contents
     *  @param where filter for the objects, defaults to all
     *
     *  @return 0 on success
     */
    int dump_acct(ostringstream& oss,
                  const string&  where,
                  int            time_start,
                  int            time_end);

    /**
     *  Dumps the VC showback information in XML format. A filter can be also
     *  added to the query as well as a time frame.
     *  @param oss the output stream to dump the pool contents
     *  @param where filter for the objects, defaults to all
     *  @param start_month First month (+year) to include. January is 1.
     *  Use -1 to unset
     *  @param start_year First year (+month) to include. e.g. 2014.
     *  Use -1 to unset
     *  @param end_month Last month (+year) to include. January is 1.
     *  Use -1 to unset
     *  @param end_year Last year (+month) to include. e.g. 2014.
     *  Use -1 to unset
     *
     *  @return 0 on success
     */
    int dump_showback(ostringstream& oss,
                      const string&  where,
                      int            start_month,
                      int            start_year,
                      int            end_month,
                      int            end_year);

    /**
     *  Dumps the VC monitoring information entries in XML format. A filter
     *  can be also added to the query.
     *
     *  @param oss the output stream to dump the pool contents
     *  @param where filter for the objects, defaults to all
     *
     *  @return 0 on success
     */
    int dump_monitoring(ostringstream& oss,
                        const string&  where);

    /**
     *  Dumps the VC monitoring information  for a single VC
     *
     *  @param oss the output stream to dump the pool contents
     *  @param vcid id of the target VC
     *
     *  @return 0 on success
     */
    int dump_monitoring(ostringstream& oss,
                        int            vcid)
    {
        ostringstream filter;

        filter << "oid = " << vcid;

        return dump_monitoring(oss, filter.str());
    }

    /**
     * Processes all the history records, and stores the monthly cost for each
     * VC
     *  @param start_month First month (+year) to process. January is 1.
     *  Use -1 to unset
     *  @param start_year First year (+month) to process. e.g. 2014.
     *  Use -1 to unset
     *  @param end_month Last month (+year) to process. January is 1.
     *  Use -1 to unset
     *  @param end_year Last year (+month) to process. e.g. 2014.
     *  Use -1 to unset
     *  @param error_str Returns the error reason, if any
     *
     *  @return 0 on success
     */
    int calculate_showback(
                int start_month,
                int start_year,
                int end_month,
                int end_year,
                string &error_str);

    /**
     * Deletes the DISK that was in the process of being attached. Releases
     * Images and updates usage quotas
     *
     * @param vid VC id
     */
    void delete_attach_disk(int vid);

    /**
     * Deletes the NIC that was in the process of being attached
     *
     * @param vid VC id
     */
    void attach_nic_failure(int vid)
    {
        delete_hotplug_nic(vid, true);
    }

    /**
     * Deletes the NIC that was in the process of being detached
     *
     * @param vid VC id
     */
    void detach_nic_success(int vid)
    {
        delete_hotplug_nic(vid, false);
    }

    /**
     * Deletes an entry in the HV-2-vcid mapping table for imported VCs
     *   @param deploy_id of the VC
     */
    void drop_index(const string& deploy_id);

private:
    /**
     *  Factory method to produce VC objects
     *    @return a pointer to the new VC
     */
    PoolObjectSQL * create()
    {
        return new VirtualCluster(-1,-1,"","","/nfs",0,0);
    };

    /**
     * Size, in seconds, of the historical monitoring information
     */
    time_t _monitor_expiration;

    /**
     * True or false whether to submit new VC on HOLD or not
     */
    bool _submit_on_hold;

    /**
     * Default values for cpu and memory cost
     */
    float _default_cpu_cost;
    float _default_mem_cost;
    float _default_disk_cost;

    /**
     * Callback used to get an int in the DB it is used by VC Pool in:
     *   - calculate_showback (min_stime)
     *   - get_vcid (vcid)
     */
    int db_int_cb(void * _min_stime, int num, char **values, char **names);

    // -------------------------------------------------------------------------
    // Virtual Machine ID - Deploy ID index for imported VCs
    // The index is managed by the VirtualMachinePool
    // -------------------------------------------------------------------------
    static const char * import_table;

    static const char * import_db_names;

    static const char * import_db_bootstrap;

    /**
     * Insert deploy_id - vcid index.
     *   @param replace will replace and not insert
     *   @return 0 on success
     */
    int insert_index(const string& deploy_id, int vc_id, bool replace);

    // -------------------------------------------------------------------------

    /**
     * Helper method for delete attach/detach
     * @param vid VC id
     * @param attach true for an attach action, false for detach
     */
    void delete_hotplug_nic(int vid, bool attach);
};

#endif /*VIRTUAL_CLUSTER_POOL_H_*/
