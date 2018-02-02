module AzDriver
    class Client
        # static
        ####################################
        #
        Storage = Azure::Storage::Profiles::Latest::Mgmt
        Network = Azure::Network::Profiles::Latest::Mgmt
        Compute = Azure::Compute::Profiles::Latest::Mgmt
        Resources = Azure::Resources::Profiles::Latest::Mgmt

        StorageModels = Storage::Models
        NetworkModels = Network::Models
        ComputeModels = Compute::Models
        ResourceModels = Resources::Models
        #
        #####################################

        # instance class
        REQUIRED_ATTRS = [:sub_id, :tenant_id, :client_id, :client_secret]
        def initialize(opts = {})
            # mandatory parameters:
            REQUIRED_ATTRS.each do |attr|
                raise "#{attr} required for initialize azure client!" if opts[attr].nil?
            end


            system = OpenNebula::System.new(OpenNebula::Client.new)
            config = system.get_configuration
            if OpenNebula.is_error?(config)
                raise "Error getting oned configuration : #{config.message}"
            end
            token = config["ONE_KEY"]

            to_decrypt = {
                :secret => opts[:client_secret]
            }

            decrypted = OpenNebula.decrypt(to_decrypt, token)

            sub_id         = opts[:sub_id] || '11111111-1111-1111-1111-111111111111'
            tenant_id      = opts[:tenant_id]
            client_id      = opts[:client_id]
            client_secret  = decrypted[:secret]

            provider = MsRestAzure::ApplicationTokenProvider.new(tenant_id, client_id, client_secret)
            credentials = MsRest::TokenCredentials.new(provider)

            options = {
                tenant_id: tenant_id,
                client_id: client_id,
                client_secret: client_secret,
                subscription_id: sub_id,
                credentials: credentials
            }

            @resource_client = Resources::Client.new(options)
            @network_client = Network::Client.new(options)
            @storage_client = Storage::Client.new(options)
            @compute_client = Compute::Client.new(options)
        end


        def resource
            @resource_client
        end

        def network
            @network_client
        end

        def storage
            @storage_client
        end

        def compute
            @compute_client
        end

    end
end
