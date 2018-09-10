$: << '../' \
   << './'

require 'opennebula'
require 'helpers/MockClient'

module OpenNebula

    describe "VirtualCluster using NOKOGIRI" do
        before(:all) do
            NOKOGIRI=true

            @xml = VirtualCluster.build_xml(6)            

            client = MockClient.new()
            @vc = VirtualCluster.new(@xml,client)
        end

        it "should create a Nokogiri Node" do
            @xml.class.to_s.should eql('Nokogiri::XML::NodeSet')
        end

        it "should allocate the new VC" do            
            @vc.allocate(nil)

            @vc.id.should eql(6)                        
        end

        it "should have VMs Ammount" do            
            @vc.vms_amt.should eql(2)
        end

        it "should have NFS Location" do
            @vc.nfs_loc.should eql("/nfs")
        end

        it "should update the VC info" do
            @vc.info()

            @vc.id.should eql(6)
            @vc.name.should eql('vc-example')
            @vc.state.should eql(3)
            @vc.vms_amt.should eql(2)
            @vc.nfs_loc.should eql("/nfs")
            @vc.state_str.should eql('ACTIVE')
            @vc.lcm_state.should eql(3)
            @vc.lcm_state_str.should eql('RUNNING')
            @vc.status.should eql('runn')
        end

        it "should deploy the VC" do
            rc = @vc.deploy(nil)

            rc.should eql(nil)
        end

        it "should migrate the VC" do
            rc = @vc.migrate(nil)

            rc.should eql(nil)
        end

        it "should live_migrate the VC" do
            rc = @vc.live_migrate(nil)

            rc.should eql(nil)
        end

        it "should access an attribute using []" do
            @vc['NAME'].should eql('vc-example')
            @vc['DEPLOY_ID'].should eql('dummy')
            @vc['TEMPLATE/MEMORY'].should eql('512')
            @vc['ID'].should eql('6')
            @vc['NAME'].should eql('vc-example')
            @vc['LCM_STATE'].should eql('3')
            @vc['DEPLOY_ID'].should eql('dummy')
            @vc['TEMPLATE/MEMORY'].should eql('512')
            @vc['TEMPLATE/CONTEXT/DNS'].should eql('192.169.1.4')
            @vc['TEMPLATE/DISK/SIZE'].should eql('1024')
            @vc['HISTORY/HOSTNAME'].should eql('dummyhost')
            @vc['HISTORY/PSTIME'].should eql('1277375186')
        end

        it "should access an attribute using to_hash" do
            vc_hash = @vc.to_hash

            vc_hash['VC']['NAME'].should eql('vc-example')
            vc_hash['VC']['DEPLOY_ID'].should eql('dummy')
            vc_hash['VC']['TEMPLATE']['MEMORY'].should eql('512')
            vc_hash['VC']['ID'].should eql('6')
            vc_hash['VC']['NAME'].should eql('vc-example')
            vc_hash['VC']['LCM_STATE'].should eql('3')
            vc_hash['VC']['DEPLOY_ID'].should eql('dummy')
            vc_hash['VC']['TEMPLATE']['MEMORY'].should eql('512')
            vc_hash['VC']['TEMPLATE']['CONTEXT']['DNS'].should eql('192.169.1.4')
            vc_hash['VC']['TEMPLATE']['DISK'][1]['SIZE'].should eql('1024')
            vc_hash['VC']['HISTORY']['HOSTNAME'].should eql('dummyhost')
            vc_hash['VC']['HISTORY']['PSTIME'].should eql('1277375186')
        end
    end

    describe "VirtualCluster using NOKOGIRI without id" do
        before(:all) do
            NOKOGIRI=true

            @xml = VirtualCluster.build_xml()

            client = MockClient.new()
            @vc = VirtualCluster.new(@xml,client)
        end

        it "should create a Nokogiri Node" do
            @xml.class.to_s.should eql('Nokogiri::XML::NodeSet')
        end

        it "should deploy the VC" do
            rc = @vc.deploy(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should migrate the VC" do
            rc = @vc.migrate(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should live_migrate the VC" do
            rc = @vc.live_migrate(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should get Error getting info" do
            rc = @vc.info()

            OpenNebula.is_error?(rc).should eql(true)
            @vc.id.should eql(nil)
            @vc.name.should eql(nil)
        end
    end 

    describe "VirtualCluster using REXML without id" do
        before(:all) do
            NOKOGIRI=false

            @xml = VirtualCluster.build_xml()

            client = MockClient.new()
            @vc = VirtualCluster.new(@xml,client)
        end

        it "should create a REXML Element" do
            @xml.class.to_s.should eql('REXML::Element')
        end

        it "should deploy the VC" do
            rc = @vc.deploy(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should migrate the VC" do
            rc = @vc.migrate(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should live_migrate the VC" do
            rc = @vc.live_migrate(nil)

            OpenNebula.is_error?(rc).should eql(true)
        end

        it "should get Error getting info" do
            rc = @vc.info()

            OpenNebula.is_error?(rc).should eql(true)
            @vc.id.should eql(nil)
            @vc.name.should eql(nil)
        end
    end
end    