
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

        def self.print_item(group)
            puts "\tName: #{group.name}"
            puts "\tId: #{group.id}"
            puts "\tLocation: #{group.location}"
            puts "\tTags: #{group.tags}"
            #print_properties(group.properties)
        end

    end
end
