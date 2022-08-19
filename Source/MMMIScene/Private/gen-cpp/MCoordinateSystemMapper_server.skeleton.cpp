// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MCoordinateSystemMapper.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::MMIStandard;

class MCoordinateSystemMapperHandler : virtual public MCoordinateSystemMapperIf {
 public:
  MCoordinateSystemMapperHandler() {
    // Your initialization goes here
  }

  void TransformToMMI_L( ::MMIStandard::MTransform& _return, const  ::MMIStandard::MTransform& transform, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("TransformToMMI_L\n");
  }

  void TransformToMMI( ::MMIStandard::MTransform& _return, const  ::MMIStandard::MTransform& transform, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("TransformToMMI\n");
  }

  void TransformFromMMI_L( ::MMIStandard::MTransform& _return, const  ::MMIStandard::MTransform& transform, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("TransformFromMMI_L\n");
  }

  void TransformFromMMI( ::MMIStandard::MTransform& _return, const  ::MMIStandard::MTransform& transform, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("TransformFromMMI\n");
  }

  void QuaternionToMMI_L( ::MMIStandard::MQuaternion& _return, const  ::MMIStandard::MQuaternion& quat, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("QuaternionToMMI_L\n");
  }

  void QuaternionToMMI( ::MMIStandard::MQuaternion& _return, const  ::MMIStandard::MQuaternion& quat, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("QuaternionToMMI\n");
  }

  void QuaternionFromMMI_L( ::MMIStandard::MQuaternion& _return, const  ::MMIStandard::MQuaternion& quat, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("QuaternionFromMMI_L\n");
  }

  void QuaternionFromMMI( ::MMIStandard::MQuaternion& _return, const  ::MMIStandard::MQuaternion& quat, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("QuaternionFromMMI\n");
  }

  void VectorToMMI_L( ::MMIStandard::MVector3& _return, const  ::MMIStandard::MVector3& quat, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("VectorToMMI_L\n");
  }

  void VectorToMMI( ::MMIStandard::MVector3& _return, const  ::MMIStandard::MVector3& quat, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("VectorToMMI\n");
  }

  void VectorFromMMI_L( ::MMIStandard::MVector3& _return, const  ::MMIStandard::MVector3& quat, const std::vector<MDirection::type> & coordinateSystem) {
    // Your implementation goes here
    printf("VectorFromMMI_L\n");
  }

  void VectorFromMMI( ::MMIStandard::MVector3& _return, const  ::MMIStandard::MVector3& quat, const MDirection::type firstAxis, const MDirection::type secondAxis, const MDirection::type thirdAxis) {
    // Your implementation goes here
    printf("VectorFromMMI\n");
  }

};


