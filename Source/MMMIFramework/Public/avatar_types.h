/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef avatar_TYPES_H
#define avatar_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>
#include "math_types.h"


namespace MMIStandard {

struct MJointType {
  enum type {
    Undefined = 0,
    LeftBallTip = 1,
    LeftBall = 2,
    LeftAnkle = 3,
    LeftKnee = 4,
    LeftHip = 5,
    RightBallTip = 6,
    RightBall = 7,
    RightAnkle = 8,
    RightKnee = 9,
    RightHip = 10,
    PelvisCentre = 11,
    S1L5Joint = 12,
    T12L1Joint = 13,
    T1T2Joint = 14,
    C4C5Joint = 15,
    HeadJoint = 16,
    HeadTip = 17,
    MidEye = 18,
    LeftShoulder = 19,
    LeftElbow = 20,
    LeftWrist = 21,
    RightShoulder = 22,
    RightElbow = 23,
    RightWrist = 24,
    LeftThumbMid = 25,
    LeftThumbMeta = 26,
    LeftThumbCarpal = 27,
    LeftThumbTip = 28,
    LeftIndexMeta = 29,
    LeftIndexProximal = 30,
    LeftIndexDistal = 31,
    LeftIndexTip = 32,
    LeftMiddleMeta = 33,
    LeftMiddleProximal = 34,
    LeftMiddleDistal = 35,
    LeftMiddleTip = 36,
    LeftRingMeta = 37,
    LeftRingProximal = 38,
    LeftRingDistal = 39,
    LeftRingTip = 40,
    LeftLittleMeta = 41,
    LeftLittleProximal = 42,
    LeftLittleDistal = 43,
    LeftLittleTip = 44,
    RightThumbMid = 45,
    RightThumbMeta = 46,
    RightThumbCarpal = 47,
    RightThumbTip = 48,
    RightIndexMeta = 49,
    RightIndexProximal = 50,
    RightIndexDistal = 51,
    RightIndexTip = 52,
    RightMiddleMeta = 53,
    RightMiddleProximal = 54,
    RightMiddleDistal = 55,
    RightMiddleTip = 56,
    RightRingMeta = 57,
    RightRingProximal = 58,
    RightRingDistal = 59,
    RightRingTip = 60,
    RightLittleMeta = 61,
    RightLittleProximal = 62,
    RightLittleDistal = 63,
    RightLittleTip = 64,
    Root = 65
  };
};

extern const std::map<int, const char*> _MJointType_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const MJointType::type& val);

std::string to_string(const MJointType::type& val);

struct MEndeffectorType {
  enum type {
    LeftHand = 0,
    LeftFoot = 1,
    RightHand = 2,
    RightFoot = 3,
    Root = 4
  };
};

extern const std::map<int, const char*> _MEndeffectorType_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const MEndeffectorType::type& val);

std::string to_string(const MEndeffectorType::type& val);

struct MChannel {
  enum type {
    XOffset = 0,
    YOffset = 1,
    ZOffset = 2,
    XRotation = 3,
    YRotation = 4,
    ZRotation = 5,
    WRotation = 6
  };
};

extern const std::map<int, const char*> _MChannel_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const MChannel::type& val);

std::string to_string(const MChannel::type& val);

class MAvatarPostureValues;

class MJoint;

class MAvatarPosture;

class MHandPose;

class MAvatarDescription;

class MAvatar;

typedef struct _MAvatarPostureValues__isset {
  _MAvatarPostureValues__isset() : PartialJointList(false) {}
  bool PartialJointList :1;
} _MAvatarPostureValues__isset;

class MMIFRAMEWORK_API MAvatarPostureValues : public virtual ::apache::thrift::TBase {
 public:

  MAvatarPostureValues(const MAvatarPostureValues&);
  MAvatarPostureValues& operator=(const MAvatarPostureValues&);
  MAvatarPostureValues() : AvatarID() {
  }

  virtual ~MAvatarPostureValues() noexcept;
  std::string AvatarID;
  std::vector<double>  PostureData;
  std::vector<MJointType::type>  PartialJointList;

  _MAvatarPostureValues__isset __isset;

  void __set_AvatarID(const std::string& val);

  void __set_PostureData(const std::vector<double> & val);

  void __set_PartialJointList(const std::vector<MJointType::type> & val);

  bool operator == (const MAvatarPostureValues & rhs) const
  {
    if (!(AvatarID == rhs.AvatarID))
      return false;
    if (!(PostureData == rhs.PostureData))
      return false;
    if (__isset.PartialJointList != rhs.__isset.PartialJointList)
      return false;
    else if (__isset.PartialJointList && !(PartialJointList == rhs.PartialJointList))
      return false;
    return true;
  }
  bool operator != (const MAvatarPostureValues &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MAvatarPostureValues & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MAvatarPostureValues &a, MAvatarPostureValues &b);

std::ostream& operator<<(std::ostream& out, const MAvatarPostureValues& obj);

typedef struct _MJoint__isset {
  _MJoint__isset() : Channels(false), Parent(false) {}
  bool Channels :1;
  bool Parent :1;
} _MJoint__isset;

class MMIFRAMEWORK_API MJoint : public virtual ::apache::thrift::TBase {
 public:

  MJoint(const MJoint&);
  MJoint& operator=(const MJoint&);
  MJoint() : ID(), Type((MJointType::type)0), Parent() {
  }

  virtual ~MJoint() noexcept;
  std::string ID;
  MJointType::type Type;
   ::MMIStandard::MVector3 Position;
   ::MMIStandard::MQuaternion Rotation;
  std::vector<MChannel::type>  Channels;
  std::string Parent;

  _MJoint__isset __isset;

  void __set_ID(const std::string& val);

  void __set_Type(const MJointType::type val);

  void __set_Position(const  ::MMIStandard::MVector3& val);

  void __set_Rotation(const  ::MMIStandard::MQuaternion& val);

  void __set_Channels(const std::vector<MChannel::type> & val);

  void __set_Parent(const std::string& val);

  bool operator == (const MJoint & rhs) const
  {
    if (!(ID == rhs.ID))
      return false;
    if (!(Type == rhs.Type))
      return false;
    if (!(Position == rhs.Position))
      return false;
    if (!(Rotation == rhs.Rotation))
      return false;
    if (__isset.Channels != rhs.__isset.Channels)
      return false;
    else if (__isset.Channels && !(Channels == rhs.Channels))
      return false;
    if (__isset.Parent != rhs.__isset.Parent)
      return false;
    else if (__isset.Parent && !(Parent == rhs.Parent))
      return false;
    return true;
  }
  bool operator != (const MJoint &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MJoint & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MJoint &a, MJoint &b);

std::ostream& operator<<(std::ostream& out, const MJoint& obj);


class MMIFRAMEWORK_API MAvatarPosture : public virtual ::apache::thrift::TBase {
 public:

  MAvatarPosture(const MAvatarPosture&);
  MAvatarPosture& operator=(const MAvatarPosture&);
  MAvatarPosture() : AvatarID() {
  }

  virtual ~MAvatarPosture() noexcept;
  std::string AvatarID;
  std::vector<MJoint>  Joints;

  void __set_AvatarID(const std::string& val);

  void __set_Joints(const std::vector<MJoint> & val);

  bool operator == (const MAvatarPosture & rhs) const
  {
    if (!(AvatarID == rhs.AvatarID))
      return false;
    if (!(Joints == rhs.Joints))
      return false;
    return true;
  }
  bool operator != (const MAvatarPosture &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MAvatarPosture & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MAvatarPosture &a, MAvatarPosture &b);

std::ostream& operator<<(std::ostream& out, const MAvatarPosture& obj);

typedef struct _MHandPose__isset {
  _MHandPose__isset() : Properties(false) {}
  bool Properties :1;
} _MHandPose__isset;

class MMIFRAMEWORK_API MHandPose : public virtual ::apache::thrift::TBase {
 public:

  MHandPose(const MHandPose&);
  MHandPose& operator=(const MHandPose&);
  MHandPose() {
  }

  virtual ~MHandPose() noexcept;
  std::vector<MJoint>  Joints;
  std::map<std::string, std::string>  Properties;

  _MHandPose__isset __isset;

  void __set_Joints(const std::vector<MJoint> & val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  bool operator == (const MHandPose & rhs) const
  {
    if (!(Joints == rhs.Joints))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    return true;
  }
  bool operator != (const MHandPose &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MHandPose & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MHandPose &a, MHandPose &b);

std::ostream& operator<<(std::ostream& out, const MHandPose& obj);

typedef struct _MAvatarDescription__isset {
  _MAvatarDescription__isset() : Properties(false) {}
  bool Properties :1;
} _MAvatarDescription__isset;

class MMIFRAMEWORK_API MAvatarDescription : public virtual ::apache::thrift::TBase {
 public:

  MAvatarDescription(const MAvatarDescription&);
  MAvatarDescription& operator=(const MAvatarDescription&);
  MAvatarDescription() : AvatarID() {
  }

  virtual ~MAvatarDescription() noexcept;
  std::string AvatarID;
  MAvatarPosture ZeroPosture;
  std::map<std::string, std::string>  Properties;

  _MAvatarDescription__isset __isset;

  void __set_AvatarID(const std::string& val);

  void __set_ZeroPosture(const MAvatarPosture& val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  bool operator == (const MAvatarDescription & rhs) const
  {
    if (!(AvatarID == rhs.AvatarID))
      return false;
    if (!(ZeroPosture == rhs.ZeroPosture))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    return true;
  }
  bool operator != (const MAvatarDescription &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MAvatarDescription & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MAvatarDescription &a, MAvatarDescription &b);

std::ostream& operator<<(std::ostream& out, const MAvatarDescription& obj);

typedef struct _MAvatar__isset {
  _MAvatar__isset() : SceneObjects(false), Properties(false) {}
  bool SceneObjects :1;
  bool Properties :1;
} _MAvatar__isset;

class MMIFRAMEWORK_API MAvatar : public virtual ::apache::thrift::TBase {
 public:

  MAvatar(const MAvatar&);
  MAvatar& operator=(const MAvatar&);
  MAvatar() : ID(), Name() {
  }

  virtual ~MAvatar() noexcept;
  std::string ID;
  std::string Name;
  MAvatarDescription Description;
  MAvatarPostureValues PostureValues;
  std::vector<std::string>  SceneObjects;
  std::map<std::string, std::string>  Properties;

  _MAvatar__isset __isset;

  void __set_ID(const std::string& val);

  void __set_Name(const std::string& val);

  void __set_Description(const MAvatarDescription& val);

  void __set_PostureValues(const MAvatarPostureValues& val);

  void __set_SceneObjects(const std::vector<std::string> & val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  bool operator == (const MAvatar & rhs) const
  {
    if (!(ID == rhs.ID))
      return false;
    if (!(Name == rhs.Name))
      return false;
    if (!(Description == rhs.Description))
      return false;
    if (!(PostureValues == rhs.PostureValues))
      return false;
    if (__isset.SceneObjects != rhs.__isset.SceneObjects)
      return false;
    else if (__isset.SceneObjects && !(SceneObjects == rhs.SceneObjects))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    return true;
  }
  bool operator != (const MAvatar &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MAvatar & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MAvatar &a, MAvatar &b);

std::ostream& operator<<(std::ostream& out, const MAvatar& obj);

} // namespace

#endif
