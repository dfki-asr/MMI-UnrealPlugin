/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef mmu_TYPES_H
#define mmu_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>
#include "core_types.h"
#include "avatar_types.h"
#include "scene_types.h"
#include "constraints_types.h"


namespace MMIStandard {

struct MCoordinateSystemType {
  enum type {
    Global = 0,
    Local = 1
  };
};

extern const std::map<int, const char*> _MCoordinateSystemType_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const MCoordinateSystemType::type& val);

std::string to_string(const MCoordinateSystemType::type& val);

struct MDependencyType {
  enum type {
    Service = 0,
    MMU = 1,
    ProgramLibrary = 2,
    MMIFramework = 3,
    Other = 4
  };
};

extern const std::map<int, const char*> _MDependencyType_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const MDependencyType::type& val);

std::string to_string(const MDependencyType::type& val);

class MSimulationState;

class MSimulationResult;

class MSimulationEvent;

class MVersion;

class MDependency;

class MMUDescription;

class MInstruction;

typedef struct _MSimulationState__isset {
  _MSimulationState__isset() : Constraints(false), SceneManipulations(false), Events(false) {}
  bool Constraints :1;
  bool SceneManipulations :1;
  bool Events :1;
} _MSimulationState__isset;

class MSimulationState : public virtual ::apache::thrift::TBase {
 public:

  MSimulationState(const MSimulationState&);
  MSimulationState& operator=(const MSimulationState&);
  MSimulationState() {
  }

  virtual ~MSimulationState() noexcept;
   ::MMIStandard::MAvatarPostureValues Initial;
   ::MMIStandard::MAvatarPostureValues Current;
  std::vector< ::MMIStandard::MConstraint>  Constraints;
  std::vector< ::MMIStandard::MSceneManipulation>  SceneManipulations;
  std::vector<MSimulationEvent>  Events;

  _MSimulationState__isset __isset;

  void __set_Initial(const  ::MMIStandard::MAvatarPostureValues& val);

  void __set_Current(const  ::MMIStandard::MAvatarPostureValues& val);

  void __set_Constraints(const std::vector< ::MMIStandard::MConstraint> & val);

  void __set_SceneManipulations(const std::vector< ::MMIStandard::MSceneManipulation> & val);

  void __set_Events(const std::vector<MSimulationEvent> & val);

  bool operator == (const MSimulationState & rhs) const
  {
    if (!(Initial == rhs.Initial))
      return false;
    if (!(Current == rhs.Current))
      return false;
    if (__isset.Constraints != rhs.__isset.Constraints)
      return false;
    else if (__isset.Constraints && !(Constraints == rhs.Constraints))
      return false;
    if (__isset.SceneManipulations != rhs.__isset.SceneManipulations)
      return false;
    else if (__isset.SceneManipulations && !(SceneManipulations == rhs.SceneManipulations))
      return false;
    if (__isset.Events != rhs.__isset.Events)
      return false;
    else if (__isset.Events && !(Events == rhs.Events))
      return false;
    return true;
  }
  bool operator != (const MSimulationState &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MSimulationState & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MSimulationState &a, MSimulationState &b);

std::ostream& operator<<(std::ostream& out, const MSimulationState& obj);

typedef struct _MSimulationResult__isset {
  _MSimulationResult__isset() : Constraints(false), Events(false), SceneManipulations(false), DrawingCalls(false), LogData(false) {}
  bool Constraints :1;
  bool Events :1;
  bool SceneManipulations :1;
  bool DrawingCalls :1;
  bool LogData :1;
} _MSimulationResult__isset;

class MSimulationResult : public virtual ::apache::thrift::TBase {
 public:

  MSimulationResult(const MSimulationResult&);
  MSimulationResult& operator=(const MSimulationResult&);
  MSimulationResult() {
  }

  virtual ~MSimulationResult() noexcept;
   ::MMIStandard::MAvatarPostureValues Posture;
  std::vector< ::MMIStandard::MConstraint>  Constraints;
  std::vector<MSimulationEvent>  Events;
  std::vector< ::MMIStandard::MSceneManipulation>  SceneManipulations;
  std::vector< ::MMIStandard::MDrawingCall>  DrawingCalls;
  std::vector<std::string>  LogData;

  _MSimulationResult__isset __isset;

  void __set_Posture(const  ::MMIStandard::MAvatarPostureValues& val);

  void __set_Constraints(const std::vector< ::MMIStandard::MConstraint> & val);

  void __set_Events(const std::vector<MSimulationEvent> & val);

  void __set_SceneManipulations(const std::vector< ::MMIStandard::MSceneManipulation> & val);

  void __set_DrawingCalls(const std::vector< ::MMIStandard::MDrawingCall> & val);

  void __set_LogData(const std::vector<std::string> & val);

  bool operator == (const MSimulationResult & rhs) const
  {
    if (!(Posture == rhs.Posture))
      return false;
    if (__isset.Constraints != rhs.__isset.Constraints)
      return false;
    else if (__isset.Constraints && !(Constraints == rhs.Constraints))
      return false;
    if (__isset.Events != rhs.__isset.Events)
      return false;
    else if (__isset.Events && !(Events == rhs.Events))
      return false;
    if (__isset.SceneManipulations != rhs.__isset.SceneManipulations)
      return false;
    else if (__isset.SceneManipulations && !(SceneManipulations == rhs.SceneManipulations))
      return false;
    if (__isset.DrawingCalls != rhs.__isset.DrawingCalls)
      return false;
    else if (__isset.DrawingCalls && !(DrawingCalls == rhs.DrawingCalls))
      return false;
    if (__isset.LogData != rhs.__isset.LogData)
      return false;
    else if (__isset.LogData && !(LogData == rhs.LogData))
      return false;
    return true;
  }
  bool operator != (const MSimulationResult &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MSimulationResult & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MSimulationResult &a, MSimulationResult &b);

std::ostream& operator<<(std::ostream& out, const MSimulationResult& obj);

typedef struct _MSimulationEvent__isset {
  _MSimulationEvent__isset() : Properties(false) {}
  bool Properties :1;
} _MSimulationEvent__isset;

class MSimulationEvent : public virtual ::apache::thrift::TBase {
 public:

  MSimulationEvent(const MSimulationEvent&);
  MSimulationEvent& operator=(const MSimulationEvent&);
  MSimulationEvent() : Name(), Type(), Reference() {
  }

  virtual ~MSimulationEvent() noexcept;
  std::string Name;
  std::string Type;
  std::string Reference;
  std::map<std::string, std::string>  Properties;

  _MSimulationEvent__isset __isset;

  void __set_Name(const std::string& val);

  void __set_Type(const std::string& val);

  void __set_Reference(const std::string& val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  bool operator == (const MSimulationEvent & rhs) const
  {
    if (!(Name == rhs.Name))
      return false;
    if (!(Type == rhs.Type))
      return false;
    if (!(Reference == rhs.Reference))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    return true;
  }
  bool operator != (const MSimulationEvent &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MSimulationEvent & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MSimulationEvent &a, MSimulationEvent &b);

std::ostream& operator<<(std::ostream& out, const MSimulationEvent& obj);

typedef struct _MVersion__isset {
  _MVersion__isset() : Minor(false), Sub(false), Subsub(false) {}
  bool Minor :1;
  bool Sub :1;
  bool Subsub :1;
} _MVersion__isset;

class MVersion : public virtual ::apache::thrift::TBase {
 public:

  MVersion(const MVersion&);
  MVersion& operator=(const MVersion&);
  MVersion() : Major(0), Minor(0), Sub(0), Subsub(0) {
  }

  virtual ~MVersion() noexcept;
  int16_t Major;
  int16_t Minor;
  int16_t Sub;
  int16_t Subsub;

  _MVersion__isset __isset;

  void __set_Major(const int16_t val);

  void __set_Minor(const int16_t val);

  void __set_Sub(const int16_t val);

  void __set_Subsub(const int16_t val);

  bool operator == (const MVersion & rhs) const
  {
    if (!(Major == rhs.Major))
      return false;
    if (__isset.Minor != rhs.__isset.Minor)
      return false;
    else if (__isset.Minor && !(Minor == rhs.Minor))
      return false;
    if (__isset.Sub != rhs.__isset.Sub)
      return false;
    else if (__isset.Sub && !(Sub == rhs.Sub))
      return false;
    if (__isset.Subsub != rhs.__isset.Subsub)
      return false;
    else if (__isset.Subsub && !(Subsub == rhs.Subsub))
      return false;
    return true;
  }
  bool operator != (const MVersion &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MVersion & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MVersion &a, MVersion &b);

std::ostream& operator<<(std::ostream& out, const MVersion& obj);

typedef struct _MDependency__isset {
  _MDependency__isset() : ExcludedVersions(false), Name(false) {}
  bool ExcludedVersions :1;
  bool Name :1;
} _MDependency__isset;

class MDependency : public virtual ::apache::thrift::TBase {
 public:

  MDependency(const MDependency&);
  MDependency& operator=(const MDependency&);
  MDependency() : ID(), Type((MDependencyType::type)0), Name() {
  }

  virtual ~MDependency() noexcept;
  std::string ID;
  MDependencyType::type Type;
  MVersion MinVersion;
  MVersion MaxVersion;
  std::vector<MVersion>  ExcludedVersions;
  std::string Name;

  _MDependency__isset __isset;

  void __set_ID(const std::string& val);

  void __set_Type(const MDependencyType::type val);

  void __set_MinVersion(const MVersion& val);

  void __set_MaxVersion(const MVersion& val);

  void __set_ExcludedVersions(const std::vector<MVersion> & val);

  void __set_Name(const std::string& val);

  bool operator == (const MDependency & rhs) const
  {
    if (!(ID == rhs.ID))
      return false;
    if (!(Type == rhs.Type))
      return false;
    if (!(MinVersion == rhs.MinVersion))
      return false;
    if (!(MaxVersion == rhs.MaxVersion))
      return false;
    if (__isset.ExcludedVersions != rhs.__isset.ExcludedVersions)
      return false;
    else if (__isset.ExcludedVersions && !(ExcludedVersions == rhs.ExcludedVersions))
      return false;
    if (__isset.Name != rhs.__isset.Name)
      return false;
    else if (__isset.Name && !(Name == rhs.Name))
      return false;
    return true;
  }
  bool operator != (const MDependency &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDependency & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MDependency &a, MDependency &b);

std::ostream& operator<<(std::ostream& out, const MDependency& obj);

typedef struct _MMUDescription__isset {
  _MMUDescription__isset() : Prerequisites(false), Properties(false), Dependencies(false), Events(false), LongDescription(false), ShortDescription(false), Parameters(false), SceneParameters(false), Vendor(false), VendorDomain(false), MmuUrl(false), UpdateUrl(false) {}
  bool Prerequisites :1;
  bool Properties :1;
  bool Dependencies :1;
  bool Events :1;
  bool LongDescription :1;
  bool ShortDescription :1;
  bool Parameters :1;
  bool SceneParameters :1;
  bool Vendor :1;
  bool VendorDomain :1;
  bool MmuUrl :1;
  bool UpdateUrl :1;
} _MMUDescription__isset;

class MMUDescription : public virtual ::apache::thrift::TBase {
 public:

  MMUDescription(const MMUDescription&);
  MMUDescription& operator=(const MMUDescription&);
  MMUDescription() : Name(), ID(), AssemblyName(), MotionType(), Language(), Author(), Version(), LongDescription(), ShortDescription(), Vendor(), VendorDomain(), MmuUrl(), UpdateUrl() {
  }

  virtual ~MMUDescription() noexcept;
  std::string Name;
  std::string ID;
  std::string AssemblyName;
  std::string MotionType;
  std::string Language;
  std::string Author;
  std::string Version;
  std::vector< ::MMIStandard::MConstraint>  Prerequisites;
  std::map<std::string, std::string>  Properties;
  std::vector<MDependency>  Dependencies;
  std::vector<std::string>  Events;
  std::string LongDescription;
  std::string ShortDescription;
  std::vector< ::MMIStandard::MParameter>  Parameters;
  std::vector< ::MMIStandard::MParameter>  SceneParameters;
  std::string Vendor;
  std::string VendorDomain;
  std::string MmuUrl;
  std::string UpdateUrl;

  _MMUDescription__isset __isset;

  void __set_Name(const std::string& val);

  void __set_ID(const std::string& val);

  void __set_AssemblyName(const std::string& val);

  void __set_MotionType(const std::string& val);

  void __set_Language(const std::string& val);

  void __set_Author(const std::string& val);

  void __set_Version(const std::string& val);

  void __set_Prerequisites(const std::vector< ::MMIStandard::MConstraint> & val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  void __set_Dependencies(const std::vector<MDependency> & val);

  void __set_Events(const std::vector<std::string> & val);

  void __set_LongDescription(const std::string& val);

  void __set_ShortDescription(const std::string& val);

  void __set_Parameters(const std::vector< ::MMIStandard::MParameter> & val);

  void __set_SceneParameters(const std::vector< ::MMIStandard::MParameter> & val);

  void __set_Vendor(const std::string& val);

  void __set_VendorDomain(const std::string& val);

  void __set_MmuUrl(const std::string& val);

  void __set_UpdateUrl(const std::string& val);

  bool operator == (const MMUDescription & rhs) const
  {
    if (!(Name == rhs.Name))
      return false;
    if (!(ID == rhs.ID))
      return false;
    if (!(AssemblyName == rhs.AssemblyName))
      return false;
    if (!(MotionType == rhs.MotionType))
      return false;
    if (!(Language == rhs.Language))
      return false;
    if (!(Author == rhs.Author))
      return false;
    if (!(Version == rhs.Version))
      return false;
    if (__isset.Prerequisites != rhs.__isset.Prerequisites)
      return false;
    else if (__isset.Prerequisites && !(Prerequisites == rhs.Prerequisites))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    if (__isset.Dependencies != rhs.__isset.Dependencies)
      return false;
    else if (__isset.Dependencies && !(Dependencies == rhs.Dependencies))
      return false;
    if (__isset.Events != rhs.__isset.Events)
      return false;
    else if (__isset.Events && !(Events == rhs.Events))
      return false;
    if (__isset.LongDescription != rhs.__isset.LongDescription)
      return false;
    else if (__isset.LongDescription && !(LongDescription == rhs.LongDescription))
      return false;
    if (__isset.ShortDescription != rhs.__isset.ShortDescription)
      return false;
    else if (__isset.ShortDescription && !(ShortDescription == rhs.ShortDescription))
      return false;
    if (__isset.Parameters != rhs.__isset.Parameters)
      return false;
    else if (__isset.Parameters && !(Parameters == rhs.Parameters))
      return false;
    if (__isset.SceneParameters != rhs.__isset.SceneParameters)
      return false;
    else if (__isset.SceneParameters && !(SceneParameters == rhs.SceneParameters))
      return false;
    if (__isset.Vendor != rhs.__isset.Vendor)
      return false;
    else if (__isset.Vendor && !(Vendor == rhs.Vendor))
      return false;
    if (__isset.VendorDomain != rhs.__isset.VendorDomain)
      return false;
    else if (__isset.VendorDomain && !(VendorDomain == rhs.VendorDomain))
      return false;
    if (__isset.MmuUrl != rhs.__isset.MmuUrl)
      return false;
    else if (__isset.MmuUrl && !(MmuUrl == rhs.MmuUrl))
      return false;
    if (__isset.UpdateUrl != rhs.__isset.UpdateUrl)
      return false;
    else if (__isset.UpdateUrl && !(UpdateUrl == rhs.UpdateUrl))
      return false;
    return true;
  }
  bool operator != (const MMUDescription &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MMUDescription & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MMUDescription &a, MMUDescription &b);

std::ostream& operator<<(std::ostream& out, const MMUDescription& obj);

typedef struct _MInstruction__isset {
  _MInstruction__isset() : Properties(false), Constraints(false), StartCondition(false), EndCondition(false), Action(false), Instructions(false) {}
  bool Properties :1;
  bool Constraints :1;
  bool StartCondition :1;
  bool EndCondition :1;
  bool Action :1;
  bool Instructions :1;
} _MInstruction__isset;

class MInstruction : public virtual ::apache::thrift::TBase {
 public:

  MInstruction(const MInstruction&);
  MInstruction& operator=(const MInstruction&);
  MInstruction() : ID(), Name(), MotionType(), AvatarID(), StartCondition(), EndCondition(), Action() {
  }

  virtual ~MInstruction() noexcept;
  std::string ID;
  std::string Name;
  std::string MotionType;
  std::string AvatarID;
  std::map<std::string, std::string>  Properties;
  std::vector< ::MMIStandard::MConstraint>  Constraints;
  std::string StartCondition;
  std::string EndCondition;
  std::string Action;
  std::vector<MInstruction>  Instructions;

  _MInstruction__isset __isset;

  void __set_ID(const std::string& val);

  void __set_Name(const std::string& val);

  void __set_MotionType(const std::string& val);

  void __set_AvatarID(const std::string& val);

  void __set_Properties(const std::map<std::string, std::string> & val);

  void __set_Constraints(const std::vector< ::MMIStandard::MConstraint> & val);

  void __set_StartCondition(const std::string& val);

  void __set_EndCondition(const std::string& val);

  void __set_Action(const std::string& val);

  void __set_Instructions(const std::vector<MInstruction> & val);

  bool operator == (const MInstruction & rhs) const
  {
    if (!(ID == rhs.ID))
      return false;
    if (!(Name == rhs.Name))
      return false;
    if (!(MotionType == rhs.MotionType))
      return false;
    if (!(AvatarID == rhs.AvatarID))
      return false;
    if (__isset.Properties != rhs.__isset.Properties)
      return false;
    else if (__isset.Properties && !(Properties == rhs.Properties))
      return false;
    if (__isset.Constraints != rhs.__isset.Constraints)
      return false;
    else if (__isset.Constraints && !(Constraints == rhs.Constraints))
      return false;
    if (__isset.StartCondition != rhs.__isset.StartCondition)
      return false;
    else if (__isset.StartCondition && !(StartCondition == rhs.StartCondition))
      return false;
    if (__isset.EndCondition != rhs.__isset.EndCondition)
      return false;
    else if (__isset.EndCondition && !(EndCondition == rhs.EndCondition))
      return false;
    if (__isset.Action != rhs.__isset.Action)
      return false;
    else if (__isset.Action && !(Action == rhs.Action))
      return false;
    if (__isset.Instructions != rhs.__isset.Instructions)
      return false;
    else if (__isset.Instructions && !(Instructions == rhs.Instructions))
      return false;
    return true;
  }
  bool operator != (const MInstruction &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MInstruction & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MInstruction &a, MInstruction &b);

std::ostream& operator<<(std::ostream& out, const MInstruction& obj);

} // namespace

#endif
