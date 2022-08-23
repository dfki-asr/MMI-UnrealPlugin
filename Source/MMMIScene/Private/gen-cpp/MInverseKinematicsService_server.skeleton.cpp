// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MInverseKinematicsService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MInverseKinematicsServiceHandler : virtual public MInverseKinematicsServiceIf {
 public:
  MInverseKinematicsServiceHandler() {
    // Your initialization goes here
  }

  void ComputeIK( ::MMIStandard::MAvatarPostureValues& _return, const  ::MMIStandard::MAvatarPostureValues& postureValues, const std::vector<MIKProperty> & properties) {
    // Your implementation goes here
    printf("ComputeIK\n");
  }

  void CalculateIKPosture(MIKServiceResult& _return, const  ::MMIStandard::MAvatarPostureValues& postureValues, const std::vector< ::MMIStandard::MConstraint> & constraints, const std::map<std::string, std::string> & properties) {
    // Your implementation goes here
    printf("CalculateIKPosture\n");
  }

};


