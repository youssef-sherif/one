module AzDriver
    class ResourceGroup
        attr_accessor :az_item

        def initialize(opts = {})
            @name   = opts[:gname]
            @region = opts[:region]
            @client = opts[:client]
            @az_item = opts[:az_item] || nil
        end

        # create the azure resource group
        def create()
            model = AzDriver::Client::ResourceModels

            resource_group_params = model::ResourceGroup.new.tap do |rg|
              rg.location = @region
            end

            @az_item = @client.resource.resource_groups.create_or_update(@name, resource_group_params)
        end

        PARAMS = ["IMAGE", "PUBLISHER", "SKU", "VM_USER", "VM_PASSWORD"]
        def create_vm(dinfo, name, nic, storage = nil)
            opts = {}
            model = AzDriver::Client::ComputeModels

            PARAMS.each do |param|
                opts[param] = dinfo.elements[param].text
            end

            vm_create_params = model::VirtualMachine.new.tap do |vm|
                vm.location = @region
                vm.os_profile = model::OSProfile.new.tap do |os_profile|
                    os_profile.computer_name = name
                    os_profile.admin_username = opts["VM_USER"]
                    os_profile.admin_password = opts["VM_PASSWORD"]
                end

                vm.storage_profile = model::StorageProfile.new.tap do |store_profile|
                    store_profile.image_reference = model::ImageReference.new.tap do |ref|
                        ref.publisher = opts["PUBLISHER"]
                        ref.offer = opts["IMAGE"]
                        ref.sku = opts["SKU"]
                        ref.version = "latest"
                    end
                    #store_profile.os_disk = model::OSDisk.new.tap do |os_disk|
                    #    os_disk.name = "os-disk-#{name}"
                    #    os_disk.caching = model::CachingTypes::None
                    #    os_disk.create_option = model::DiskCreateOptionTypes::FromImage
                    #    os_disk.vhd = model::VirtualHardDisk.new.tap do |vhd|
                    #        vhd.uri = "https://#{storage_acct.name}.blob.core.windows.net/rubycontainer/#{name}.vhd"
                    #    end
                    #end
                end

                vm.hardware_profile = model::HardwareProfile.new.tap do |hardware|
                    hardware.vm_size = model::VirtualMachineSizeTypes::StandardDS2V2
                end

                vm.network_profile = model::NetworkProfile.new.tap do |net_profile|
                    net_profile.network_interfaces = [
                        model::NetworkInterfaceReference.new.tap do |ref|
                            ref.id = nic.id
                            ref.primary = true
                        end
                    ]
                end
            end

            vm = @client.compute.virtual_machines.create_or_update(@name, name, vm_create_params)
        end

        # create a virtual network in the resource group
        def create_net(opts = {})
            name        = opts[:name] || 'one_vnet'
            addr_prefix = opts[:addr_prefix] || '10.0.0.0/16'
            dns         = opts[:dns] || '8.8.8.8'
            subname     = opts[:subname] || 'default'
            sub_prefix  = opts[:sub_prefix] || '10.0.0.0/24'

            model = AzDriver::Client::NetworkModels
            vnet_create_params = model::VirtualNetwork.new.tap do |vnet|
                vnet.location = @region
                vnet.address_space = model::AddressSpace.new.tap do |addr_space|
                    addr_space.address_prefixes = [addr_prefix]
                end
                vnet.dhcp_options = model::DhcpOptions.new.tap do |dhcp|
                    dhcp.dns_servers = [dns]
                end
                vnet.subnets = [
                    model::Subnet.new.tap do |subnet|
                        subnet.name = subname
                        subnet.address_prefix = sub_prefix
                    end
                ]
            end

            @client.network.virtual_networks.create_or_update(@name, name, vnet_create_params)
        end

        def print_machines()
            @client.compute.virtual_machines.list(@name).each do |vm|
                puts vm.instance_view
                puts
                puts
                AzDriver::Helper.print_item(vm)
            end
        end

        def monitor_vms()
            work_q = Queue.new
            @client.compute.virtual_machines.list(@name).each do |vm|
                work_q.push AzDriver::VirtualMachine.new(@client.compute, @name, vm)
            end
            workers = (0...10).map do
                Thread.new do
                    begin
                        while i = work_q.pop(true)
                            i.info
                            if i.status
                                puts i.status.code
                            end
                        end
                    rescue ThreadError => e
                    rescue Exception => e
                        raise e.message
                    end
                end
            end; "ok"
            workers.map(&:join); "ok"
        end

        def get_vm(name)
            object = @client.compute.virtual_machines.get(@name, name, 'InstanceView')
            return AzDriver::VirtualMachine.new(@client.compute, @name, object)
        end
    end
end
