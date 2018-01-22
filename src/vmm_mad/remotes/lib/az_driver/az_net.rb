module AzDriver
    class Network
        attr_accessor :az_item

        def initialize(opts = {})
            @group_name = opts[:gname]
            @client = opts[:client].network || opts[:client] || nil
            @az_item = opts[:az_item] || nil
        end

        def create_nic(vm_name)
            location = @az_item.location
            model = AzDriver::Client.model("network")
            nic_name = "nic-#{vm_name}"
            nic = @client.network_interfaces.create_or_update(
                @group_name,
                nic_name,
                model::NetworkInterface.new.tap do |interface|
                    interface.location = location
                    interface.ip_configurations = [
                        model::NetworkInterfaceIPConfiguration.new.tap do |nic_conf|
                            nic_conf.name = nic_name
                            nic_conf.private_ipallocation_method = model::IPAllocationMethod::Dynamic
                            nic_conf.subnet = az_item.subnets.first
                        end
                    ]
                end
            )
        end
    end
end
