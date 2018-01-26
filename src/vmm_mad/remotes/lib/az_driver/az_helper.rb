
module AzDriver
    class Config
        attr_reader  :public_cloud_az_conf
        attr_reader  :instance_types
        attr_reader  :to_inst

        def initialize()
            @to_inst ={}
            @public_cloud_az_conf  = YAML::load(File.read(AZ_DRIVER_CONF))

            if @public_cloud_az_conf['proxy_uri']
                ENV['HTTP_PROXY'] = @public_cloud_az_conf['proxy_uri']
            end

            @instance_types = @public_cloud_az_conf['instance_types']
            @instance_types.keys.each{ |key|
                @to_inst[key.upcase] = key
            }
        end

        def instance_type_capacity(name)
            resource = @instance_types[@to_inst[name]] || @instance_types[name]
            return 0, 0 if resource.nil?
            return (resource['cpu'].to_f * 100).to_i ,
                   (resource['memory'].to_f * 1024 * 1024).to_i
        end
    end

        def self.host_credentials(one_host)
            opts = {}
            opts[:sub_id]        = one_host["TEMPLATE/AZ_SUB"]
            opts[:client_id]     = one_host["TEMPLATE/AZ_CLIENT"]
            opts[:client_secret] = one_host["TEMPLATE/AZ_SECRET"]
            opts[:tenant_id]     = one_host["TEMPLATE/AZ_TENANT"]

            opts
        end

        def self.host_credentials_with_name(host)

            pool = OpenNebula::HostPool.new(OpenNebula::Client.new)
            pool.info

            opts = {}
            opts[:sub_id]        = pool["/HOST_POOL/HOST[NAME='#{host}']/TEMPLATE/AZ_SUB"]
            opts[:client_id]     = pool["/HOST_POOL/HOST[NAME='#{host}']/TEMPLATE/AZ_CLIENT"]
            opts[:client_secret] = pool["/HOST_POOL/HOST[NAME='#{host}']/TEMPLATE/AZ_SECRET"]
            opts[:tenant_id]     = pool["/HOST_POOL/HOST[NAME='#{host}']/TEMPLATE/AZ_TENANT"]

            opts
        end

        def self.retrieve_host(name)
            pool = OpenNebula::HostPool.new(OpenNebula::Client.new)
            pool.info
            objects=pool.select {|object| object.name == name }
            objects.first
        end

        def self.host_credentials_with_id(host_id)
            begin
                host = OpenNebula::Host.new_with_id(host_id, OpenNebula::Client.new)
                rc = host.info
                if OpenNebula.is_error?(rc)
                    raise "Could not get host info for ID: #{host_id} - #{rc.message}"
                end

                opts = host_credentials(host)
            rescue Exception => e
                raise e
            end
        end


        def self.print_item(resource)
		    resource.instance_variables.sort.each do |ivar|
		        str = ivar.to_s.gsub /^@/, ''
		        if resource.respond_to? str.to_sym
		          puts "\t\t#{str}: #{resource.send(str.to_sym)}"
		        end
		    end
		    puts "\n\n"
		end

        def self.print_group(group)
            puts "\tName: #{group.name}"
            puts "\tId: #{group.id}"
            puts "\tLocation: #{group.location}"
            puts "\tTags: #{group.tags}"
            print_item(group.properties)
        end

        def self.get_deployment_info(host, xml_text, region = nil)
            xml = REXML::Document.new xml_text
            az = nil
            all_az_elements = xml.root.get_elements("//USER_TEMPLATE/PUBLIC_CLOUD")

            # Look for an azure location
            # if we find the same LOCATION as @region name
            # means that we have the final location
            all_az_elements.each { |element|
                type = element.elements["TYPE"].text.downcase
                location = element.elements["LOCATION"].text.downcase rescue nil

                next if type  != "azure"

                if location.nil?
                    az = element
                elsif location && location == location.downcase
                    az = element
                    break
                end
            }

            # If we don't find an Azure location raise an error
            if !az
                raise "Cannot find Azure element in VM template "<<
                      "or couldn't find any Azure location matching "<<
                      "one of the templates."
            end

            # If LOCATION not explicitly defined, try to get from host, if not
            # try to use hostname as datacenter
            if !az.elements["LOCATION"]
                location=REXML::Element.new("LOCATION")
                location.text = region  || host
                az.elements << location
            end

            az
        end

        # Build template for importation
        def self.vm_to_one(vm, host, config)
            cpu, mem = config.instance_type_capacity(vm.size)

            mem = mem.to_i / 1024 # Memory for templates expressed in MB
            cpu = cpu.to_f / 100  # CPU expressed in units

            vm.set_resources(cpu, mem)

            str = "NAME   = \"Instance from #{vm.name}\"\n"\
                  "CPU    = \"#{cpu}\"\n"\
                  "vCPU   = \"#{cpu.ceil}\"\n"\
                  "MEMORY = \"#{mem}\"\n"\
                  "HYPERVISOR = \"AZURE\"\n"\
                  "PUBLIC_CLOUD = [\n"\
                  "  TYPE  =\"azure\"\n"\
                  "]\n"\
                  "IMPORT_VM_ID    = \"#{vm.name}\"\n"\
                  "SCHED_REQUIREMENTS=\"NAME=\\\"#{host}\\\"\"\n"\
                  "DESCRIPTION = \"Instance imported from Azure, from instance"\
                  " #{vm.name}\"\n"

            str
        end
end
