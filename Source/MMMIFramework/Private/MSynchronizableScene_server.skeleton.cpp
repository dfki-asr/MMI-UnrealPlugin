// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "MSynchronizableScene.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MSynchronizableSceneHandler : virtual public MSynchronizableSceneIf {
 public:
  MSynchronizableSceneHandler() {
    // Your initialization goes here
  }

  void ApplyUpdates( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MSceneUpdate& sceneUpdates) {
    // Your implementation goes here
    printf("ApplyUpdates\n");
  }

  void ApplyManipulations( ::MMIStandard::MBoolResponse& _return, const std::vector< ::MMIStandard::MSceneManipulation> & sceneManipulations) {
    // Your implementation goes here
    printf("ApplyManipulations\n");
  }

};


