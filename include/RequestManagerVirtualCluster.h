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

#ifndef REQUEST_MANAGER_VIRTUAL_CLUSTER_H_
#define REQUEST_MANAGER_VIRTUAL_CLUSTER_H

#include "Request.h"
#include "Nebula.h"

using namespace std;

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class RequestManagerVirtualCluster: public Request
{
protected:
    RequestManagerVirtualCluster(const string& method_name,
                       const string& help,
                       const string& params)
        :Request(method_name, params, help)
    {
        Nebula& nd  = Nebula::instance();
        pool        = nd.get_vcpool();

        auth_object = PoolObjectSQL::VC;
        auth_op     = AuthRequest::MANAGE;
    };

    ~RequestManagerVirtualCluster(){};

    /* -------------------------------------------------------------------- */

    virtual void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att) = 0;

    bool vc_authorization(int id,
                          ImageTemplate *         tmpl,
                          VirtualClusterTemplate* vctmpl,
                          RequestAttributes&      att,
                          PoolObjectAuth *        host_perms,
                          PoolObjectAuth *        ds_perm,
                          PoolObjectAuth *        img_perm,
                          AuthRequest::Operation  op);

    bool quota_resize_authorization(
            Template *          deltas,
            RequestAttributes&  att,
            PoolObjectAuth&     vc_perms);

    bool quota_resize_authorization(
            int                 oid,
            Template *          deltas,
            RequestAttributes&  att);

    int get_host_information(
        int     hid,
        string& name,
        string& vcm,
        int&    cluster_id,
        bool&   is_public_cloud,
        PoolObjectAuth&    host_perms,
        RequestAttributes& att);

    int get_ds_information(
        int ds_id,
        set<int>& ds_cluster_ids,
        string& tm_mad,
        RequestAttributes& att,
        bool& ds_migr);

    int get_default_ds_information(
        int cluster_id,
        int& ds_id,
        string& tm_mad,
        RequestAttributes& att);

    bool check_host(int hid,
                    bool enforce,
                    VirtualCluster* vm,
                    string& error);

    int add_history(VirtualCluster * vm,
                    int              hid,
                    int              cid,
                    const string&    hostname,
                    const string&    vmm_mad,
                    const string&    tm_mad,
                    int              ds_id,
                    RequestAttributes& att);

    VirtualCluster * get_vc(int id, RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterAction : public RequestManagerVirtualCluster
{
public:
    //auth_op is MANAGE for all actions but "resched" and "unresched"
    //this is dynamically set for each request in the execute method
    VirtualClusterAction():
        RequestManagerVirtualCluster("one.vc.action",
                                     "Performs an action on a virtual cluster",
                                     "A:ssi"){};
    ~VirtualClusterAction(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterDeploy : public RequestManagerVirtualCluster
{
public:
    VirtualClusterDeploy():
        RequestManagerVirtualCluster("one.vc.deploy",
                                     "Deploys a virtual cluster",
                                     "A:siibi")
    {
        auth_op = Nebula::instance().get_vm_auth_op(History::DEPLOY_ACTION);
    };

    ~VirtualClusterDeploy(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterMigrate : public RequestManagerVirtualCluster
{
public:
    VirtualClusterMigrate():
        RequestManagerVirtualCluster("one.vc.migrate",
                                     "Migrates a virtual cluster",
                                     "A:siibbi"){
        auth_op = Nebula::instance().get_vm_auth_op(History::MIGRATE_ACTION);
    };

    ~VirtualClusterMigrate(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterDiskSaveas : public RequestManagerVirtualCluster
{
public:
    VirtualClusterDiskSaveas():
        RequestManagerVirtualCluster("one.vc.disksaveas",
                           "Save a disk from virtual cluster as a new image",
                           "A:siissi"){
        auth_op= Nebula::instance().get_vm_auth_op(History::DISK_SAVEAS_ACTION);
    };

    ~VirtualClusterDiskSaveas(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterMonitoring : public RequestManagerVirtualCluster
{
public:

    VirtualClusterMonitoring():
        RequestManagerVirtualCluster("one.vc.monitoring",
                "Returns the virtual cluster monitoring records",
                "A:si"){
        auth_op = AuthRequest::USE_NO_LCK;
    };

    ~VirtualClusterMonitoring(){};

    void request_execute(
            xmlrpc_c::paramList const& paramList, RequestAttributes& att);

    virtual bool is_locked(xmlrpc_c::paramList const& paramList, RequestAttributes& att){
        return false;
    };
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterAttach : public RequestManagerVirtualCluster
{
public:
    VirtualClusterAttach():
        RequestManagerVirtualCluster("one.vc.attach",
                           "Attaches a new disk to the virtual cluster",
                           "A:sis"){
        auth_op= Nebula::instance().get_vm_auth_op(History::DISK_ATTACH_ACTION);
    };

    ~VirtualClusterAttach(){};

    /**
     * Process a DISK attahment request to a Virtual Machine
     *   @param id of the VirtualCluster
     *   @param tl with the new DISK description
     *   @param att attributes of this request
     *   @return ErroCode as defined in Request
     */
    ErrorCode request_execute(int id, VirtualClusterTemplate& tl,
        RequestAttributes& att);

protected:

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterDetach : public RequestManagerVirtualCluster
{
public:
    VirtualClusterDetach():
        RequestManagerVirtualCluster("one.vc.detach",
                           "Detaches a disk from a virtual cluster",
                           "A:sii"){
        //Attach & detach are set to the same auth op in OpenNebulaTemplate
        auth_op= Nebula::instance().get_vm_auth_op(History::DISK_DETACH_ACTION);
    };

    ~VirtualClusterDetach(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterAttachNic : public RequestManagerVirtualCluster
{
public:
    VirtualClusterAttachNic():
        RequestManagerVirtualCluster("one.vc.attachnic",
                           "Attaches a new NIC to the virtual cluster",
                           "A:sis"){
        auth_op = Nebula::instance().get_vm_auth_op(History::NIC_ATTACH_ACTION);
    };

    ~VirtualClusterAttachNic(){};

    /**
     * Process a NIC attahment request to a Virtual Machine
     *   @param id of the VirtualCluster
     *   @param tl with the new NIC description
     *   @param att attributes of this request
     *   @return ErroCode as defined in Request
     */
    ErrorCode request_execute(int id, VirtualClusterTemplate& tl,
        RequestAttributes& att);

protected:

    void request_execute(xmlrpc_c::paramList const& pl, RequestAttributes& ra);

};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterDetachNic : public RequestManagerVirtualCluster
{
public:
    VirtualClusterDetachNic():
        RequestManagerVirtualCluster("one.vc.detachnic",
                           "Detaches a NIC from a virtual cluster",
                           "A:sii"){
        //Attach & detach are set to the same auth op in OpenNebulaTemplate
        auth_op = Nebula::instance().get_vm_auth_op(History::NIC_DETACH_ACTION);
    };

    ~VirtualClusterDetachNic(){};

    /**
     * Process a NIC detach request to a Virtual Machine
     *   @param id of the VirtualCluster
     *   @param nic_id id of the NIC
     *   @param att attributes of this request
     *   @return ErroCode as defined in Request
     */
    ErrorCode request_execute(int id, int nic_id, RequestAttributes& att);

protected:
    void request_execute(xmlrpc_c::paramList const& pl, RequestAttributes& ra);
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterResize : public RequestManagerVirtualCluster
{
public:
    VirtualClusterResize():
        RequestManagerVirtualCluster("one.vc.resize",
                           "Changes the capacity of the virtual cluster",
                           "A:sisb"){
        auth_op = Nebula::instance().get_vm_auth_op(History::RESIZE_ACTION);
    };

    ~VirtualClusterResize(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterSnapshotCreate: public RequestManagerVirtualCluster
{
public:
    VirtualClusterSnapshotCreate():
        RequestManagerVirtualCluster("one.vc.snapshotcreate",
                           "Creates a new virtual cluster snapshot",
                           "A:sis"){
        Nebula& nd = Nebula::instance();

        //All VM snapshot operations are set to the same auth value
        auth_op    = nd.get_vm_auth_op(History::SNAPSHOT_CREATE_ACTION);
    };

    ~VirtualClusterSnapshotCreate(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterSnapshotRevert: public RequestManagerVirtualCluster
{
public:
    VirtualClusterSnapshotRevert():
        RequestManagerVirtualCluster("one.vc.snapshotrevert",
                           "Reverts a virtual cluster to a snapshot",
                           "A:sii"){
        Nebula& nd = Nebula::instance();

        //All VM snapshot operations are set to the same auth value
        auth_op    = nd.get_vm_auth_op(History::SNAPSHOT_REVERT_ACTION);
    };

    ~VirtualClusterSnapshotRevert(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterSnapshotDelete: public RequestManagerVirtualCluster
{
public:
    VirtualClusterSnapshotDelete():
        RequestManagerVirtualCluster("one.vc.snapshotdelete",
                           "Deletes a virtual cluster snapshot",
                           "A:sii"){
        Nebula& nd = Nebula::instance();

        //All VM snapshot operations are set to the same auth value
        auth_op    = nd.get_vm_auth_op(History::SNAPSHOT_DELETE_ACTION);
    };

    ~VirtualClusterSnapshotDelete(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

class VirtualClusterRecover: public RequestManagerVirtualCluster
{
public:
    VirtualClusterRecover():
        RequestManagerVirtualCluster("one.vc.recover",
                                     "Recovers a virtual cluster",
                                     "A:sii"){};
    ~VirtualClusterRecover(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterPoolCalculateShowback : public RequestManagerVirtualCluster
{
public:

    VirtualClusterPoolCalculateShowback():
        RequestManagerVirtualCluster("one.vcpool.calculateshowback",
            "Processes all the history records, and stores the monthly cost"
            " for each VC", "A:sii")
    {
        Nebula& nd  = Nebula::instance();
        pool        = nd.get_vcpool();

        auth_object = PoolObjectSQL::VC;
    };

    ~VirtualClusterPoolCalculateShowback(){};

    /* -------------------------------------------------------------------- */

    void request_execute(
            xmlrpc_c::paramList const& paramList, RequestAttributes& att);
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterDiskSnapshotCreate: public RequestManagerVirtualCluster
{
public:
    VirtualClusterDiskSnapshotCreate():
        RequestManagerVirtualCluster("one.vc.disksnapshotcreate",
                           "Creates a new virtual cluster disk snapshot",
                           "A:siis"){
        Nebula& nd  = Nebula::instance();
        ipool       = nd.get_ipool();

        //All VM disk snapshot operations are set to the same auth value
        auth_op = nd.get_vm_auth_op(History::DISK_SNAPSHOT_CREATE_ACTION);
    };

    ~VirtualClusterDiskSnapshotCreate(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);

private:
    ImagePool* ipool;
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterDiskSnapshotRevert: public RequestManagerVirtualCluster
{
public:
    VirtualClusterDiskSnapshotRevert():
        RequestManagerVirtualCluster("one.vc.disksnapshotrevert",
                           "Reverts disk state to a snapshot",
                           "A:siii"){
        Nebula& nd = Nebula::instance();

        //All VM disk snapshot operations are set to the same auth value
        auth_op = nd.get_vm_auth_op(History::DISK_SNAPSHOT_REVERT_ACTION);
    };

    ~VirtualClusterDiskSnapshotRevert(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterDiskSnapshotDelete: public RequestManagerVirtualCluster
{
public:
    VirtualClusterDiskSnapshotDelete():
        RequestManagerVirtualCluster("one.vc.disksnapshotdelete",
                           "Deletes a disk snapshot",
                           "A:siii"){
        Nebula& nd  = Nebula::instance();
        ipool       = nd.get_ipool();

        //All VM disk snapshot operations are set to the same auth value
        auth_op = nd.get_vm_auth_op(History::DISK_SNAPSHOT_DELETE_ACTION);
    };

    ~VirtualClusterDiskSnapshotDelete(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);

private:
    ImagePool* ipool;
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

class VirtualClusterUpdateConf: public RequestManagerVirtualCluster
{
public:
    VirtualClusterUpdateConf():
        RequestManagerVirtualCluster("one.vc.updateconf",
                           "Updates several configuration attributes of a VC",
                           "A:sis"){
        auth_op = Nebula::instance().get_vm_auth_op(History::UPDATECONF_ACTION);
    };

    ~VirtualClusterUpdateConf(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
};

class VirtualClusterDiskResize : public RequestManagerVirtualCluster
{
public:
    VirtualClusterDiskResize():
        RequestManagerVirtualCluster("one.vc.diskresize",
                           "Resizes a disk from a virtual cluster",
                           "A:siis"){
        Nebula& nd = Nebula::instance();
        ipool      = nd.get_ipool();

        auth_op = nd.get_vm_auth_op(History::DISK_RESIZE_ACTION);
    };

    ~VirtualClusterDiskResize(){};

    void request_execute(xmlrpc_c::paramList const& _paramList,
            RequestAttributes& att);
private:
    ImagePool* ipool;
};

#endif
