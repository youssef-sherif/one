$: << '../' \
   << './'

require 'opennebula'
require 'helpers/MockClient'

module OpenNebula

    describe "VirtualClusterPool using NOKOGIRI" do
        before(:all) do
            NOKOGIRI=true

            client = MockClient.new()
            @vc_pool = VirtualClusterPool.new(client)
        end

        it "should update the VC_POOL info" do
            rc = @vc_pool.info()
            rc.nil?.should eql(true)
        end

        # it "should iterate the VC_POOL elements and get info from them using to_hash" do
        #     vc_pool = @vc_pool.to_hash

        #     vc_pool['VC_POOL']['VC'].each{ |vc|
        #         if vc['ID'] == 6
        #             vc.name.should eql('vc-example')
        #             vc.state.should eql(3)
        #             vc.state_str.should eql('ACTIVE')
        #             vc['UID'].should eql('0')
        #             vc['USERNAME'].should eql('oneadmin')
        #             vc['LAST_POLL'].should eql('1277910006')
        #             vc['HISTORY']['HOSTNAME'].should eql('dummyhost')
        #             vc['HISTORY']['STIME'].should eql('1277375186')
        #             vc['HISTORY']['REASON'].should eql('0')
        #         elsif vc['ID'] == 8
        #             vc.name.should eql('vcext')
        #             vc.state.should eql(4)
        #             vc.state_str.should eql('STOPPED')
        #             vc['UID'].should eql('0')
        #             vc['USERNAME'].should eql('oneadmin')
        #             vc['LAST_POLL'].should eql('1277910006')
        #             vc['HISTORY']['HOSTNAME'].should eql('thost')
        #             vc['HISTORY']['STIME'].should eql('1277377556')
        #             vc['HISTORY']['REASON'].should eql('0')
        #         end
        #     }
        # end

        it "should iterate the VC_POOL elements and get info from them" do
            rc = @vc_pool.each{ |vc|
                vc.class.to_s.should eql("OpenNebula::VirtualCluster")
                if vc.id == 6
                    vc.name.should eql('vc-example')
                    vc.state.should eql(3)
                    vc.state_str.should eql('ACTIVE')
                    vc['UID'].should eql('0')
                    vc['USERNAME'].should eql('oneadmin')
                    vc['LAST_POLL'].should eql('1277910006')
                    vc['HISTORY/HOSTNAME'].should eql('dummyhost')
                    vc['HISTORY/STIME'].should eql('1277375186')
                    vc['HISTORY/REASON'].should eql('0')
                elsif vc.id == 8
                    vc.name.should eql('vcext')
                    vc.state.should eql(4)
                    vc.state_str.should eql('STOPPED')
                    vc['UID'].should eql('0')
                    vc['USERNAME'].should eql('oneadmin')
                    vc['LAST_POLL'].should eql('1277910006')
                    vc['HISTORY/HOSTNAME'].should eql('thost')
                    vc['HISTORY/STIME'].should eql('1277377556')
                    vc['HISTORY/REASON'].should eql('0')
                end
            }
        end
    end

    describe "VirtualClusterPool using REXML" do
        before(:all) do
            NOKOGIRI=false

            client = MockClient.new()
            @vc_pool = VirtualClusterPool.new(client)
        end

        it "should update the VC_POOL info" do
            rc = @vc_pool.info()
            rc.nil?.should eql(true)
        end

        it "should iterate the VC_POOL elements and get info from them" do
            rc = @vc_pool.each{ |vc|
                vc.class.to_s.should eql("OpenNebula::VirtualCluster")
                if vc.id == 6
                    vc.name.should eql('vc-example')
                    vc.state.should eql(3)
                    vc.state_str.should eql('ACTIVE')
                    vc['UID'].should eql('0')
                    vc['USERNAME'].should eql('oneadmin')
                    vc['LAST_POLL'].should eql('1277910006')
                    vc['HISTORY/HOSTNAME'].should eql('dummyhost')
                    vc['HISTORY/STIME'].should eql('1277375186')
                    vc['HISTORY/REASON'].should eql('0')
                elsif vc.id == 8
                    vc.name.should eql('vcext')
                    vc.state.should eql(4)
                    vc.state_str.should eql('STOPPED')
                    vc['UID'].should eql('0')
                    vc['USERNAME'].should eql('oneadmin')
                    vc['LAST_POLL'].should eql('1277910006')
                    vc['HISTORY/HOSTNAME'].should eql('thost')
                    vc['HISTORY/STIME'].should eql('1277377556')
                    vc['HISTORY/REASON'].should eql('0')
                end
            }
        end
    end
end
