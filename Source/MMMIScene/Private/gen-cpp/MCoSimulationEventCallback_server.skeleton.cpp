// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MCoSimulationEventCallback.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MCoSimulationEventCallbackHandler : virtual public MCoSimulationEventCallbackIf {
 public:
  MCoSimulationEventCallbackHandler() {
    // Your initialization goes here
  }

  void OnEvent(const MCoSimulationEvents& event) {
    // Your implementation goes here
    printf("OnEvent\n");
  }

  void OnFrameEnd(const  ::MMIStandard::MAvatarPostureValues& newPosture) {
    // Your implementation goes here
    printf("OnFrameEnd\n");
  }

};


