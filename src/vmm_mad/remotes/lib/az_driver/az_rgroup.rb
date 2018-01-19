module AzDriver
    class ResourceGroup
        def initialize(client, name)
            @client = client
            @name   = name
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
