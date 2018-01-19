module AzDriver
    class VirtualMachine
        def initialize(client, group, object)
            @client = client
            @az_item = object
            @group_name = group
        end

		def print()
            AzDriver::Helper.print_item(@az_item)
		end

        def status
            @status unless @status.nil?
        end

        def info()
            view = @client.virtual_machines.instance_view(@group_name, @az_item.name)
            @status = view.statuses[1]
        end

        def start()
            @client.virtual_machines.start(@group_name, @az_item.name)
        end

        def stop()
            @client.virtual_machines.power_off(@group_name, @az_item.name)
        end

        def poweroff()
            @client.virtual_machines.deallocate(@group_name, @az_item.name)
        end
    end
end
