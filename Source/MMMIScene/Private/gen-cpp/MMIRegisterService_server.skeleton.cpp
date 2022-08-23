// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MMIRegisterService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MMIRegisterServiceHandler : virtual public MMIRegisterServiceIf {
 public:
  MMIRegisterServiceHandler() {
    // Your initialization goes here
  }

  void GetRegisteredAdapters(std::vector<MAdapterDescription> & _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetRegisteredAdapters\n");
  }

  void GetRegisteredServices(std::vector< ::MMIStandard::MServiceDescription> & _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetRegisteredServices\n");
  }

  void GetAvailableMMUs(std::map< ::MMIStandard::MMUDescription, std::vector< ::MMIStandard::MIPAddress> > & _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetAvailableMMUs\n");
  }

  void RegisterAdapter( ::MMIStandard::MBoolResponse& _return, const MAdapterDescription& adapterDescription) {
    // Your implementation goes here
    printf("RegisterAdapter\n");
  }

  void UnregisterAdapter( ::MMIStandard::MBoolResponse& _return, const MAdapterDescription& adapterDescription) {
    // Your implementation goes here
    printf("UnregisterAdapter\n");
  }

  void RegisterService( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MServiceDescription& serviceDescription) {
    // Your implementation goes here
    printf("RegisterService\n");
  }

  void UnregisterService( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MServiceDescription& serviceDescription) {
    // Your implementation goes here
    printf("UnregisterService\n");
  }

  void CreateSessionID(std::string& _return, const std::map<std::string, std::string> & properties) {
    // Your implementation goes here
    printf("CreateSessionID\n");
  }


};


