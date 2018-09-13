$: << '../' \
   << './'

require 'opennebula'
require 'helpers/MockClient'

module OpenNebula
    describe "VCTemplate using NOKOGIRI" do
        before(:all) do
            NOKOGIRI=true

            @xml = VCTemplate.build_xml(6)            

            client = MockClient.new()
            @vc = VCTemplate.new(@xml,client)
        end

        it "should create a Nokogiri Node" do
            @xml.class.to_s.should eql('Nokogiri::XML::NodeSet')
        end

        it "should allocate the new VCTemplate" do            
            @vc.allocate(nil)       
            @vc.id.should eql(5)     
        end

        it "should update the VCTemplate info" do
            @vc.info()
            
            @vc.name.should eql('test-vc')            
        end
    end


end