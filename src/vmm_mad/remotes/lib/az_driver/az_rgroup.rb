module AzDriver
    class ResourceGroup
        def initialize(client, name)
            @client = client
            @name   = name
        end

        def print_machines()
            @client.compute.virtual_machines.list(@name).each do |vm|
                AzDriver::Helper.print_item(vm)
            end
        end

    end
end
