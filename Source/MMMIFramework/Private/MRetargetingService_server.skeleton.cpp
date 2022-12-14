// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "MRetargetingService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MRetargetingServiceHandler : virtual public MRetargetingServiceIf {
 public:
  MRetargetingServiceHandler() {
    // Your initialization goes here
  }

  void SetupRetargeting( ::MMIStandard::MAvatarDescription& _return, const  ::MMIStandard::MAvatarPosture& globalTarget) {
    // Your implementation goes here
    printf("SetupRetargeting\n");
  }

  void RetargetToIntermediate( ::MMIStandard::MAvatarPostureValues& _return, const  ::MMIStandard::MAvatarPosture& globalTarget) {
    // Your implementation goes here
    printf("RetargetToIntermediate\n");
  }

  void RetargetToTarget( ::MMIStandard::MAvatarPosture& _return, const  ::MMIStandard::MAvatarPostureValues& intermediatePostureValues) {
    // Your implementation goes here
    printf("RetargetToTarget\n");
  }

};

