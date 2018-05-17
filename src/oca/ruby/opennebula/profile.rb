# -------------------------------------------------------------------------- #
# Copyright 2002-2017, OpenNebula Project, OpenNebula Systems                #
#                                                                            #
# Licensed under the Apache License, Version 2.0 (the "License"); you may    #
# not use this file except in compliance with the License. You may obtain    #
# a copy of the License at                                                   #
#                                                                            #
# http://www.apache.org/licenses/LICENSE-2.0                                 #
#                                                                            #
# Unless required by applicable law or agreed to in writing, software        #
# distributed under the License is distributed on an "AS IS" BASIS,          #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
# See the License for the specific language governing permissions and        #
# limitations under the License.                                             #
#--------------------------------------------------------------------------- #

module OpenNebula
    ONE_LOCATION = ENV["ONE_LOCATION"] if !defined?(ONE_LOCATION)

    if !ONE_LOCATION
        VAR_LOCATION = "/var/lib/one/" if !defined?(VAR_LOCATION)
    else
        VAR_LOCATION = ONE_LOCATION + "/var/" if !defined?(VAR_LOCATION)
    end

    PROFILE_PATH = VAR_LOCATION + "/oca_profile.log"
    REFERENCE    = Time.now

    def self.profile(msg, &pb)
    begin
        file = File.open(PROFILE_PATH, 'a')

        t1   = Time.now
        rc   = yield
        t2   = Time.now

        file << "#{msg}: #{t2-REFERENCE} #{t2 - t1}\n"

        return rc
    ensure
        file.close
    end
    end
end
