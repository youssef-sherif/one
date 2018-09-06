require 'opennebula/pool_element'

module OpenNebula
    class VirtualMachine < PoolElement
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
            :lock     => "vc.lock",
            :unlock     => "vc.unlock"
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

        SHORT_VM_STATES={
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
    end
end        