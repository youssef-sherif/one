
module AzDriver
    class Helper

        def self.host_credentials(one_host)
            opts = {}
            opts[:sub_id]        = one_host["TEMPLATE/AZ_SUB"]
            opts[:client_id]     = one_host["TEMPLATE/AZ_CLIENT"]
            opts[:client_secret] = one_host["TEMPLATE/AZ_SECRET"]
            opts[:tenant_id]     = one_host["TEMPLATE/AZ_TENANT"]

            opts
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

    end
end
