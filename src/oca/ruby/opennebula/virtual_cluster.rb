# -------------------------------------------------------------------------- #
# Copyright 2002-2018, OpenNebula Project, OpenNebula Systems                #
#                                                                            #
# Licensed under the Apache License, Version 2.0 (the "License"); you may    #
# not use this file except in compliance with the License. You may obtain    #
# a copy of the License at                                                   #
#                                                                            #
# http://www.apache.org/licenses/LICENSE-2.0                                 #
#                                                                            #
# Unless required by applicable law or agreed to in writing, software        #
# distributed under the License is distributed on an "AS IS" BASIS,          #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
# See the License for the specific language governing permissions and        #
# limitations under the License.                                             #
#--------------------------------------------------------------------------- #

require 'opennebula/pool_element'

module OpenNebula
    class VirtualCluster < PoolElement
        VC_METHODS = {
            :info           => "vc.info",
            :allocate       => "vc.allocate",
            :action         => "vc.action",
            :migrate        => "vc.migrate",
            :deploy         => "vc.deploy",
            :chown          => "vc.chown",
            :chmod          => "vc.chmod",
            :monitoring     => "vc.monitoring",
            :attach         => "vc.attach",
            :detach         => "vc.detach",
            :rename         => "vc.rename",
            :update         => "vc.update",
            :resize         => "vc.resize",
            :snapshotcreate => "vc.snapshotcreate",
            :snapshotrevert => "vc.snapshotrevert",
            :snapshotdelete => "vc.snapshotdelete",
            :attachnic      => "vc.attachnic",
            :detachnic      => "vc.detachnic",
            :recover        => "vc.recover",            
            :updateconf     => "vc.updateconf",
            :lock           => "vc.lock",
            :unlock         => "vc.unlock"
        }

        VC_STATE=%w{INIT PENDING HOLD ACTIVE STOPPED SUSPENDED DONE FAILED
            POWEROFF UNDEPLOYED CLONING CLONING_FAILURE}

        LCM_STATE=%w{
            LCM_INIT
            PROLOG
            BOOT
            RUNNING
            MIGRATE
            SAVE_STOP
            SAVE_SUSPEND
            SAVE_MIGRATE
            PROLOG_MIGRATE
            PROLOG_RESUME
            EPILOG_STOP
            EPILOG
            SHUTDOWN
            CANCEL
            FAILURE
            CLEANUP_RESUBMIT
            UNKNOWN
            HOTPLUG
            SHUTDOWN_POWEROFF
            BOOT_UNKNOWN
            BOOT_POWEROFF
            BOOT_SUSPENDED
            BOOT_STOPPED
            CLEANUP_DELETE
            HOTPLUG_SNAPSHOT
            HOTPLUG_NIC
            HOTPLUG_SAVEAS
            HOTPLUG_SAVEAS_POWEROFF
            HOTPLUG_SAVEAS_SUSPENDED
            SHUTDOWN_UNDEPLOY
            EPILOG_UNDEPLOY
            PROLOG_UNDEPLOY
            BOOT_UNDEPLOY
            HOTPLUG_PROLOG_POWEROFF
            HOTPLUG_EPILOG_POWEROFF
            BOOT_MIGRATE
            BOOT_FAILURE
            BOOT_MIGRATE_FAILURE
            PROLOG_MIGRATE_FAILURE
            PROLOG_FAILURE
            EPILOG_FAILURE
            EPILOG_STOP_FAILURE
            EPILOG_UNDEPLOY_FAILURE
            PROLOG_MIGRATE_POWEROFF
            PROLOG_MIGRATE_POWEROFF_FAILURE
            PROLOG_MIGRATE_SUSPEND
            PROLOG_MIGRATE_SUSPEND_FAILURE
            BOOT_UNDEPLOY_FAILURE
            BOOT_STOPPED_FAILURE
            PROLOG_RESUME_FAILURE
            PROLOG_UNDEPLOY_FAILURE
            PROLOG_MIGRATE_UNKNOWN
            PROLOG_MIGRATE_UNKNOWN_FAILURE            
        }        

        SHORT_VC_STATES={
            "INIT"              => "init",
            "PENDING"           => "pend",
            "HOLD"              => "hold",
            "ACTIVE"            => "actv",
            "STOPPED"           => "stop",
            "SUSPENDED"         => "susp",
            "DONE"              => "done",
            "FAILED"            => "fail",
            "POWEROFF"          => "poff",
            "UNDEPLOYED"        => "unde",
            "CLONING"           => "clon",
            "CLONING_FAILURE"   => "fail"
        }

        SHORT_LCM_STATES={
            "PROLOG"            => "prol",
            "BOOT"              => "boot",
            "RUNNING"           => "runn",
            "MIGRATE"           => "migr",
            "SAVE_STOP"         => "save",
            "SAVE_SUSPEND"      => "save",
            "SAVE_MIGRATE"      => "save",
            "PROLOG_MIGRATE"    => "migr",
            "PROLOG_RESUME"     => "prol",
            "EPILOG_STOP"       => "epil",
            "EPILOG"            => "epil",
            "SHUTDOWN"          => "shut",
            "CANCEL"            => "shut",
            "FAILURE"           => "fail",
            "CLEANUP_RESUBMIT"  => "clea",
            "UNKNOWN"           => "unkn",
            "HOTPLUG"           => "hotp",
            "SHUTDOWN_POWEROFF" => "shut",
            "BOOT_UNKNOWN"      => "boot",
            "BOOT_POWEROFF"     => "boot",
            "BOOT_SUSPENDED"    => "boot",
            "BOOT_STOPPED"      => "boot",
            "CLEANUP_DELETE"    => "clea",
            "HOTPLUG_SNAPSHOT"  => "snap",
            "HOTPLUG_NIC"       => "hotp",
            "HOTPLUG_SAVEAS"           => "hotp",
            "HOTPLUG_SAVEAS_POWEROFF"  => "hotp",
            "HOTPLUG_SAVEAS_SUSPENDED" => "hotp",
            "SHUTDOWN_UNDEPLOY" => "shut",
            "EPILOG_UNDEPLOY"   => "epil",
            "PROLOG_UNDEPLOY"   => "prol",
            "BOOT_UNDEPLOY"     => "boot",
            "HOTPLUG_PROLOG_POWEROFF"   => "hotp",
            "HOTPLUG_EPILOG_POWEROFF"   => "hotp",
            "BOOT_MIGRATE"              => "boot",
            "BOOT_FAILURE"              => "fail",
            "BOOT_MIGRATE_FAILURE"      => "fail",
            "PROLOG_MIGRATE_FAILURE"    => "fail",
            "PROLOG_FAILURE"            => "fail",
            "EPILOG_FAILURE"            => "fail",
            "EPILOG_STOP_FAILURE"       => "fail",
            "EPILOG_UNDEPLOY_FAILURE"   => "fail",
            "PROLOG_MIGRATE_POWEROFF"   => "migr",
            "PROLOG_MIGRATE_POWEROFF_FAILURE"   => "fail",
            "PROLOG_MIGRATE_SUSPEND"            => "migr",
            "PROLOG_MIGRATE_SUSPEND_FAILURE"    => "fail",
            "BOOT_UNDEPLOY_FAILURE"     => "fail",
            "BOOT_STOPPED_FAILURE"      => "fail",
            "PROLOG_RESUME_FAILURE"     => "fail",
            "PROLOG_UNDEPLOY_FAILURE"   => "fail",
            "PROLOG_MIGRATE_UNKNOWN" => "migr",
            "PROLOG_MIGRATE_UNKNOWN_FAILURE" => "fail",
        }        
        HISTORY_ACTION=%w{none migrate live-migrate shutdown shutdown-hard
            undeploy undeploy-hard hold release stop suspend resume boot delete
            delete-recreate reboot reboot-hard resched unresched poweroff
            poweroff-hard disk-attach disk-detach nic-attach nic-detach
            disk-snapshot-create disk-snapshot-delete terminate terminate-hard
            disk-resize deploy chown chmod updateconf rename resize update
            snapshot-resize snapshot-delete snapshot-revert disk-saveas
            disk-snapshot-revert recover retry monitor}

        EXTERNAL_IP_ATTRS = [
            'GUEST_IP',
            'AWS_IP_ADDRESS',
            'AWS_PUBLIC_IP_ADDRESS',
            'AWS_PRIVATE_IP_ADDRESS',
            'AZ_IPADDRESS',
            'SL_PRIMARYIPADDRESS'
        ]

        # VirtualMachineDriver constants
        module Driver
            POLL_ATTRIBUTE = {
                :memory          => "MEMORY",
                :cpu             => "CPU",
                :nettx           => "NETTX",
                :netrx           => "NETRX",
                :state           => "STATE",
                :disk_size       => "DISK_SIZE",
                :snapshot_size   => "SNAPSHOT_SIZE"
            }

            VC_STATE = {
                :active  => 'a',
                :paused  => 'p',
                :error   => 'e',
                :deleted => 'd',
                :unknown => '-'
            }
        end

        # Creates a VirtualCluster description with just its identifier
        # this method should be used to create plain VirtualMachine objects.
        # +id+ the id of the vc
        def VirtualCluster.build_xml(pe_id=nil)
            if pe_id
                vc_xml = "<VC><ID>#{pe_id}</ID></VC>"
            else
                vc_xml = "<VC></VC>"
            end

            XMLElement.build_xml(vc_xml, 'VC')
        end

        def VirtualCluster.get_history_action(action)
            return HISTORY_ACTION[action.to_i]
        end

        # Class constructor
        # Number of Virtual Machines defaults to 2
        # NFS shared location defaults to /nfs
        def initialize(xml, client)
            super(xml,client)             
        end

        #######################################################################
        # XML-RPC Methods for the Virtual Machine Object
        #######################################################################

        # Retrieves the information of the given VirtualCluster.
        def info()
            super(VC_METHODS[:info], 'VC')
        end

        alias_method :info!, :info

        # Allocates a new VirtualCluster in OpenNebula
        #
        # @param description [String] A string containing the template of
        #   the VirtualCluster.
        # @param hold [true,false] false to create the VC in pending state,
        #   true to create it on hold
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def allocate(description, vms_amount=2, nfs_location="/nfs", hold=false)            
            super(VC_METHODS[:allocate], description, vms_amount, nfs_location, hold)
        end

        # Replaces the template contents
        #
        # @param new_template [String] New template contents
        # @param append [true, false] True to append new attributes instead of
        #   replace the whole template
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def update(new_template=nil, append=false)
            super(VC_METHODS[:update], new_template, append ? 1 : 0)
        end

        # Returns the <USER_TEMPLATE> element in text form
        #
        # @param indent [true,false] indents the resulting string, defaults to true
        #
        # @return [String] The USER_TEMPLATE
        def user_template_str(indent=true)
            template_like_str('USER_TEMPLATE', indent)
        end

        # Returns the <USER_TEMPLATE> element in XML form
        #
        # @return [String] The USER_TEMPLATE
        def user_template_xml
            if NOKOGIRI
                @xml.xpath('USER_TEMPLATE').to_s
            else
                @xml.elements['USER_TEMPLATE'].to_s
            end
        end

        def replace(opts = {})
            super(opts, "USER_TEMPLATE")
        end

        # Initiates the instance of the VC on the target host.
        #
        # @param host_id [Interger] The host id (hid) of the target host where
        #   the VC will be instantiated.
        # @param enforce [true|false] If it is set to true, the host capacity
        #   will be checked, and the deployment will fail if the host is
        #   overcommited. Defaults to false
        # @param ds_id [Integer] The System Datastore where to deploy the VC. To
        #   use the default, set it to -1
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def deploy(host_id, enforce=false, ds_id=-1)
            enforce ||= false
            ds_id ||= -1

            self.info

            return call(VC_METHODS[:deploy],
                        @pe_id,
                        host_id.to_i,
                        enforce,
                        ds_id.to_i)
        end

        # Shutdowns an already deployed VC
        def terminate(hard=false)
            action(hard ? 'terminate-hard' : 'terminate')
        end

        alias_method :shutdown, :terminate

        # Shuts down an already deployed VC, saving its state in the system DS
        def undeploy(hard=false)
            action(hard ? 'undeploy-hard' : 'undeploy')
        end

        # Powers off a running VC
        def poweroff(hard=false)
            action(hard ? 'poweroff-hard' : 'poweroff')
        end

        # Reboots an already deployed VC
        def reboot(hard=false)
            action(hard ? 'reboot-hard' : 'reboot')
        end

        # Sets a VC to hold state, scheduler will not deploy it
        def hold
            action('hold')
        end

        # Releases a VC from hold state
        def release
            action('release')
        end

        # Stops a running VC
        def stop
            action('stop')
        end

        # Saves a running VC
        def suspend
            action('suspend')
        end

        # Resumes the execution of a saved VC
        def resume
            action('resume')
        end

        # Attaches a disk to a running VC
        #
        # @param disk_template [String] Template containing a DISK element
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def disk_attach(disk_template)
            return call(VC_METHODS[:attach], @pe_id, disk_template)
        end

        alias_method :attachdisk, :disk_attach

        # Detaches a disk from a running VC
        #
        # @param disk_id [Integer] Id of the disk to be detached
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def disk_detach(disk_id)
            return call(VC_METHODS[:detach], @pe_id, disk_id)
        end

        alias_method :detachdisk, :disk_detach

        # Attaches a NIC to a running VC
        #
        # @param nic_template [String] Template containing a NIC element
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def nic_attach(nic_template)
            return call(VC_METHODS[:attachnic], @pe_id, nic_template)
        end

        # Detaches a NIC from a running VC
        #
        # @param nic_id [Integer] Id of the NIC to be detached
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def nic_detach(nic_id)
            return call(VC_METHODS[:detachnic], @pe_id, nic_id)
        end

        # Sets the re-scheduling flag for the VC
        def resched
            action('resched')
        end

        # Unsets the re-scheduling flag for the VC
        def unresched
            action('unresched')
        end

        # Moves a running VC to the specified host. With live=true the
        # migration is done withdout downtime.
        #
        # @param host_id [Interger] The host id (hid) of the target host where
        #   the VC will be migrated.
        # @param live [true|false] If true the migration is done without
        #   downtime. Defaults to false
        # @param enforce [true|false] If it is set to true, the host capacity
        #   will be checked, and the deployment will fail if the host is
        #   overcommited. Defaults to false
        # @param ds_id [Integer] The System Datastore where to migrate the VC.
        #   To use the current one, set it to -1
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def migrate(host_id, live=false, enforce=false, ds_id=-1)
            call(VC_METHODS[:migrate], @pe_id, host_id.to_i, live==true,
                enforce, ds_id.to_i)
        end

        # @deprecated use {#migrate} instead
        def live_migrate(host_id, enforce=false)
            migrate(host_id, true, enforce)
        end

        # Resize the VC
        #
        # @param capacity_template [String] Template containing the new capacity
        #   elements CPU, VCPU, MEMORY. If one of them is not present, or its
        #   value is 0, it will not be resized
        # @param enforce [true|false] If it is set to true, the host capacity
        #   will be checked. This will only affect oneadmin requests, regular users
        #   resize requests will always be enforced
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def resize(capacity_template, enforce)
            return call(VC_METHODS[:resize], @pe_id, capacity_template, enforce)
        end

        # Changes the owner/group
        # uid:: _Integer_ the new owner id. Set to -1 to leave the current one
        # gid:: _Integer_ the new group id. Set to -1 to leave the current one
        # [return] nil in case of success or an Error object
        def chown(uid, gid)
            super(VC_METHODS[:chown], uid, gid)
        end

        # Changes the permissions.
        #
        # @param octet [String] Permissions octed , e.g. 640
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def chmod_octet(octet)
            super(VC_METHODS[:chmod], octet)
        end

        # Changes the permissions.
        # Each [Integer] argument must be 1 to allow, 0 deny, -1 do not change
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def chmod(owner_u, owner_m, owner_a, group_u, group_m, group_a, other_u,
                other_m, other_a)
            super(VC_METHODS[:chmod], owner_u, owner_m, owner_a, group_u,
                group_m, group_a, other_u, other_m, other_a)
        end

        # Retrieves this VC's monitoring data from OpenNebula
        #
        # @param [Array<String>] xpath_expressions Elements to retrieve.
        #
        # @return [Hash<String, Array<Array<int>>>, OpenNebula::Error] Hash with
        #   the requested xpath expressions, and an Array of 'timestamp, value'.
        def monitoring(xpath_expressions)
            return super(VC_METHODS[:monitoring], 'VC',
                'LAST_POLL', xpath_expressions)
        end

        # Retrieves this VC's monitoring data from OpenNebula, in XML
        #
        # @return [String] VC monitoring data, in XML
        def monitoring_xml()
            return Error.new('ID not defined') if !@pe_id

            return @client.call(VC_METHODS[:monitoring], @pe_id)
        end

        # Renames this VC
        #
        # @param name [String] New name for the VC.
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def rename(name)
            return call(VC_METHODS[:rename], @pe_id, name)
        end

        # Creates a new VC snapshot
        #
        # @param name [String] Name for the snapshot.
        #
        # @return [Integer, OpenNebula::Error] The new snaphost ID in case
        #   of success, Error otherwise
        def snapshot_create(name="")
            return Error.new('ID not defined') if !@pe_id

            name ||= ""
            return @client.call(VC_METHODS[:snapshotcreate], @pe_id, name)
        end

        # Reverts to a snapshot
        #
        # @param snap_id [Integer] Id of the snapshot
        #
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def snapshot_revert(snap_id)
            return call(VC_METHODS[:snapshotrevert], @pe_id, snap_id)
        end

        # Recovers an ACTIVE VC
        #
        # @param result [Integer] Recover with failure (0), success (1),
        # retry (2), delete (3), delete-recreate (4)
        # @param result [info] Additional information needed to recover the VC
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
        def recover(result)
            return call(VC_METHODS[:recover], @pe_id, result)
        end

        # Deletes a VC from the pool
        def delete(recreate=false)
            if recreate
                recover(4)
            else
                recover(3)
            end
        end

		#  Changes the attributes of a VC in power off, failure and undeploy
		#  states
		#  @param new_conf, string describing the new attributes. Each attribute
        #  will replace the existing ones or delete it if empty. Attributes that
        #  can be updated are: INPUT/{TYPE, BUS}; RAW/{TYPE, DATA, DATA_VCX},
        #  OS/{BOOT, BOOTLOADER, ARCH, MACHINE, KERNEL, INITRD},
        #  FEATURES/{ACPI, APIC, PAE, LOCALTIME, HYPERV, GUEST_AGENT},
        #  and GRAPHICS/{TYPE, LISTEN, PASSWD, KEYMAP}
        # @return [nil, OpenNebula::Error] nil in case of success, Error
        #   otherwise
		def updateconf(new_conf)
            return call(VC_METHODS[:updateconf], @pe_id, new_conf)
        end

        # Lock a VC
        def lock(level)
            return call(VC_METHODS[:lock], @pe_id, level)
        end

        # Unlock a VC
        def unlock()
            return call(VC_METHODS[:unlock], @pe_id)
        end

        ########################################################################
        # Helpers to get VirtualCluster information
        ########################################################################

        # Returns the VC state of the VirtualCluster (numeric value)
        def state
            self['STATE'].to_i
        end

        # Returns the VC state of the VirtualCluster (string value)
        def state_str
            VC_STATE[state]
        end

        # Returns the LCM state of the VirtualCluster (numeric value)
        def lcm_state
            self['LCM_STATE'].to_i
        end

        # Returns the LCM state of the VirtualCluster (string value)
        def lcm_state_str
            LCM_STATE[lcm_state]
        end

        # Returns the short status string for the VirtualCluster
        def status
            short_state_str=SHORT_VC_STATES[state_str]

            if short_state_str=="actv"
                short_state_str=SHORT_LCM_STATES[lcm_state_str]
            end

            short_state_str
        end

        # Returns number of Virtual Machines
        def vms_amount
            self['VMS_AMT'].to_i
        end

        # Returns the NFS shared file system location
        def nfs_location
            self['NFS_LOC'].to_s
        end

        # Returns the group identifier
        # [return] _Integer_ the element's group ID
        def gid
            self['GID'].to_i
        end

        # Returns the deploy_id of the VirtualMachine (numeric value)
        def deploy_id
            self['DEPLOY_ID']
        end

        # Clones the VM's source Template, replacing the disks with live snapshots
        # of the current disks. The VM capacity and NICs are also preserved
        #
        # @param name [String] Name for the new Template
        # @param name [true,false,nil] Optional, true to make the saved images
        # persistent, false make them non-persistent
        #
        # @return [Integer, OpenNebula::Error] the new Template ID in case of
        #   success, error otherwise
        REMOVE_VNET_ATTRS = %w{AR_ID BRIDGE CLUSTER_ID IP MAC TARGET NIC_ID
            NETWORK_ID VN_MAD SECURITY_GROUPS VLAN_ID}


        def wait_lcm_state(state, timeout=10)
            vc_state = ""
            lcm_state = ""

            timeout.times do
                rc = info()
                return rc if OpenNebula.is_error?(rc)

                vc_state = state_str()
                lcm_state = lcm_state_str()

                if lcm_state == state
                    return true
                end

                sleep 1
            end

            return Error.new("Timeout expired for state #{state}. "<<
                "VC is in state #{vc_state}, #{lcm_state}")
        end
    end
end    