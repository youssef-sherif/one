module AzDriver
    class VirtualMachine

        attr_accessor :az_item
        attr_accessor :name

        def initialize(opts = {})
            @group_name = opts[:gname]
            @name = opts[:name]

            @client = opts[:client].compute || opts[:client] || nil
            @az_item = opts[:az_item] || nil
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
