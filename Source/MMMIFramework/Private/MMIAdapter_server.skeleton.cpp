// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "MMIAdapter.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MMIAdapterHandler : virtual public MMIAdapterIf {
 public:
  MMIAdapterHandler() {
    // Your initialization goes here
  }

  void Initialize( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MAvatarDescription& avatarDescription, const std::map<std::string, std::string> & properties, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("Initialize\n");
  }

  void AssignInstruction( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MInstruction& instruction, const  ::MMIStandard::MSimulationState& simulationState, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("AssignInstruction\n");
  }

  void DoStep( ::MMIStandard::MSimulationResult& _return, const double time, const  ::MMIStandard::MSimulationState& simulationState, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("DoStep\n");
  }

  void GetBoundaryConstraints(std::vector< ::MMIStandard::MConstraint> & _return, const  ::MMIStandard::MInstruction& instruction, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetBoundaryConstraints\n");
  }

  void CheckPrerequisites( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MInstruction& instruction, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("CheckPrerequisites\n");
  }

  void Abort( ::MMIStandard::MBoolResponse& _return, const std::string& instructionID, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("Abort\n");
  }

  void Dispose( ::MMIStandard::MBoolResponse& _return, const std::string& mmuID, const std::string& sessionID, const std::string& avatarID) {
    // Your implementation goes here
    printf("Dispose\n");
  }

  void ExecuteFunction(std::map<std::string, std::string> & _return, const std::string& name, const std::map<std::string, std::string> & parameters, const std::string& mmuID, const std::string& sessionID, const std::string& avatarID) {
    // Your implementation goes here
    printf("ExecuteFunction\n");
  }

  void GetStatus(std::map<std::string, std::string> & _return) {
    // Your implementation goes here
    printf("GetStatus\n");
  }

  void GetAdapterDescription(MAdapterDescription& _return) {
    // Your implementation goes here
    printf("GetAdapterDescription\n");
  }

  void CreateSession( ::MMIStandard::MBoolResponse& _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("CreateSession\n");
  }

  void CloseSession( ::MMIStandard::MBoolResponse& _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("CloseSession\n");
  }

  void PushScene( ::MMIStandard::MBoolResponse& _return, const  ::MMIStandard::MSceneUpdate& sceneUpdates, const std::string& sessionID) {
    // Your implementation goes here
    printf("PushScene\n");
  }

  void GetLoadableMMUs(std::vector< ::MMIStandard::MMUDescription> & _return) {
    // Your implementation goes here
    printf("GetLoadableMMUs\n");
  }

  void GetMMus(std::vector< ::MMIStandard::MMUDescription> & _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetMMus\n");
  }

  void GetDescription( ::MMIStandard::MMUDescription& _return, const std::string& mmuID, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetDescription\n");
  }

  void GetScene(std::vector< ::MMIStandard::MSceneObject> & _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetScene\n");
  }

  void GetSceneChanges( ::MMIStandard::MSceneUpdate& _return, const std::string& sessionID) {
    // Your implementation goes here
    printf("GetSceneChanges\n");
  }

  void LoadMMUs(std::map<std::string, std::string> & _return, const std::vector<std::string> & mmus, const std::string& sessionID) {
    // Your implementation goes here
    printf("LoadMMUs\n");
  }

  void CreateCheckpoint(std::string& _return, const std::string& mmuID, const std::string& sessionID, const std::string& avatarID) {
    // Your implementation goes here
    printf("CreateCheckpoint\n");
  }

  void RestoreCheckpoint( ::MMIStandard::MBoolResponse& _return, const std::string& mmuID, const std::string& sessionID, const std::string& checkpointData, const std::string& avatarID) {
    // Your implementation goes here
    printf("RestoreCheckpoint\n");
  }

};


