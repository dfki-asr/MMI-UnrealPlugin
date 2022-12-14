/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "core_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace MMIStandard {


MIPAddress::~MIPAddress() noexcept {
}


void MIPAddress::__set_Address(const std::string& val) {
  this->Address = val;
}

void MIPAddress::__set_Port(const int32_t val) {
  this->Port = val;
}
std::ostream& operator<<(std::ostream& out, const MIPAddress& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MIPAddress::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Address = false;
  bool isset_Port = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Address);
          isset_Address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Port);
          isset_Port = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_Address)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Port)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MIPAddress::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MIPAddress");

  xfer += oprot->writeFieldBegin("Address", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Port", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->Port);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MIPAddress &a, MIPAddress &b) {
  using ::std::swap;
  swap(a.Address, b.Address);
  swap(a.Port, b.Port);
}

MIPAddress::MIPAddress(const MIPAddress& other0) {
  Address = other0.Address;
  Port = other0.Port;
}
MIPAddress& MIPAddress::operator=(const MIPAddress& other1) {
  Address = other1.Address;
  Port = other1.Port;
  return *this;
}
void MIPAddress::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MIPAddress(";
  out << "Address=" << to_string(Address);
  out << ", " << "Port=" << to_string(Port);
  out << ")";
}


MBoolResponse::~MBoolResponse() noexcept {
}


void MBoolResponse::__set_Successful(const bool val) {
  this->Successful = val;
}

void MBoolResponse::__set_LogData(const std::vector<std::string> & val) {
  this->LogData = val;
__isset.LogData = true;
}
std::ostream& operator<<(std::ostream& out, const MBoolResponse& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MBoolResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Successful = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->Successful);
          isset_Successful = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->LogData.clear();
            uint32_t _size2;
            ::apache::thrift::protocol::TType _etype5;
            xfer += iprot->readListBegin(_etype5, _size2);
            this->LogData.resize(_size2);
            uint32_t _i6;
            for (_i6 = 0; _i6 < _size2; ++_i6)
            {
              xfer += iprot->readString(this->LogData[_i6]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.LogData = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_Successful)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MBoolResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MBoolResponse");

  xfer += oprot->writeFieldBegin("Successful", ::apache::thrift::protocol::T_BOOL, 1);
  xfer += oprot->writeBool(this->Successful);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.LogData) {
    xfer += oprot->writeFieldBegin("LogData", ::apache::thrift::protocol::T_LIST, 2);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->LogData.size()));
      std::vector<std::string> ::const_iterator _iter7;
      for (_iter7 = this->LogData.begin(); _iter7 != this->LogData.end(); ++_iter7)
      {
        xfer += oprot->writeString((*_iter7));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MBoolResponse &a, MBoolResponse &b) {
  using ::std::swap;
  swap(a.Successful, b.Successful);
  swap(a.LogData, b.LogData);
  swap(a.__isset, b.__isset);
}

MBoolResponse::MBoolResponse(const MBoolResponse& other8) {
  Successful = other8.Successful;
  LogData = other8.LogData;
  __isset = other8.__isset;
}
MBoolResponse& MBoolResponse::operator=(const MBoolResponse& other9) {
  Successful = other9.Successful;
  LogData = other9.LogData;
  __isset = other9.__isset;
  return *this;
}
void MBoolResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MBoolResponse(";
  out << "Successful=" << to_string(Successful);
  out << ", " << "LogData="; (__isset.LogData ? (out << to_string(LogData)) : (out << "<null>"));
  out << ")";
}


MParameter::~MParameter() noexcept {
}


void MParameter::__set_Name(const std::string& val) {
  this->Name = val;
}

void MParameter::__set_Type(const std::string& val) {
  this->Type = val;
}

void MParameter::__set_Description(const std::string& val) {
  this->Description = val;
}

void MParameter::__set_Required(const bool val) {
  this->Required = val;
}
std::ostream& operator<<(std::ostream& out, const MParameter& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MParameter::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Name = false;
  bool isset_Type = false;
  bool isset_Description = false;
  bool isset_Required = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          isset_Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Type);
          isset_Type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Description);
          isset_Description = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->Required);
          isset_Required = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_Name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Description)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Required)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MParameter::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MParameter");

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Type", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->Type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Description", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->Description);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Required", ::apache::thrift::protocol::T_BOOL, 4);
  xfer += oprot->writeBool(this->Required);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MParameter &a, MParameter &b) {
  using ::std::swap;
  swap(a.Name, b.Name);
  swap(a.Type, b.Type);
  swap(a.Description, b.Description);
  swap(a.Required, b.Required);
}

MParameter::MParameter(const MParameter& other10) {
  Name = other10.Name;
  Type = other10.Type;
  Description = other10.Description;
  Required = other10.Required;
}
MParameter& MParameter::operator=(const MParameter& other11) {
  Name = other11.Name;
  Type = other11.Type;
  Description = other11.Description;
  Required = other11.Required;
  return *this;
}
void MParameter::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MParameter(";
  out << "Name=" << to_string(Name);
  out << ", " << "Type=" << to_string(Type);
  out << ", " << "Description=" << to_string(Description);
  out << ", " << "Required=" << to_string(Required);
  out << ")";
}


MExecutableDescription::~MExecutableDescription() noexcept {
}


void MExecutableDescription::__set_Name(const std::string& val) {
  this->Name = val;
}

void MExecutableDescription::__set_ID(const std::string& val) {
  this->ID = val;
}

void MExecutableDescription::__set_Language(const std::string& val) {
  this->Language = val;
}

void MExecutableDescription::__set_ExecutableName(const std::string& val) {
  this->ExecutableName = val;
}

void MExecutableDescription::__set_Author(const std::string& val) {
  this->Author = val;
}

void MExecutableDescription::__set_Version(const std::string& val) {
  this->Version = val;
}

void MExecutableDescription::__set_Dependencies(const std::vector<std::string> & val) {
  this->Dependencies = val;
__isset.Dependencies = true;
}

void MExecutableDescription::__set_Vendor(const std::string& val) {
  this->Vendor = val;
__isset.Vendor = true;
}

void MExecutableDescription::__set_VendorDomain(const std::string& val) {
  this->VendorDomain = val;
__isset.VendorDomain = true;
}

void MExecutableDescription::__set_ServiceUrl(const std::string& val) {
  this->ServiceUrl = val;
__isset.ServiceUrl = true;
}

void MExecutableDescription::__set_UpdateUrl(const std::string& val) {
  this->UpdateUrl = val;
__isset.UpdateUrl = true;
}
std::ostream& operator<<(std::ostream& out, const MExecutableDescription& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MExecutableDescription::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Name = false;
  bool isset_ID = false;
  bool isset_Language = false;
  bool isset_ExecutableName = false;
  bool isset_Author = false;
  bool isset_Version = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          isset_Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ID);
          isset_ID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Language);
          isset_Language = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ExecutableName);
          isset_ExecutableName = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Author);
          isset_Author = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Version);
          isset_Version = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Dependencies.clear();
            uint32_t _size12;
            ::apache::thrift::protocol::TType _etype15;
            xfer += iprot->readListBegin(_etype15, _size12);
            this->Dependencies.resize(_size12);
            uint32_t _i16;
            for (_i16 = 0; _i16 < _size12; ++_i16)
            {
              xfer += iprot->readString(this->Dependencies[_i16]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.Dependencies = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Vendor);
          this->__isset.Vendor = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->VendorDomain);
          this->__isset.VendorDomain = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ServiceUrl);
          this->__isset.ServiceUrl = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 11:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->UpdateUrl);
          this->__isset.UpdateUrl = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_Name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ID)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Language)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ExecutableName)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Author)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Version)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MExecutableDescription::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MExecutableDescription");

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ID", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->ID);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Language", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->Language);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ExecutableName", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->ExecutableName);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Author", ::apache::thrift::protocol::T_STRING, 5);
  xfer += oprot->writeString(this->Author);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Version", ::apache::thrift::protocol::T_STRING, 6);
  xfer += oprot->writeString(this->Version);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.Dependencies) {
    xfer += oprot->writeFieldBegin("Dependencies", ::apache::thrift::protocol::T_LIST, 7);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->Dependencies.size()));
      std::vector<std::string> ::const_iterator _iter17;
      for (_iter17 = this->Dependencies.begin(); _iter17 != this->Dependencies.end(); ++_iter17)
      {
        xfer += oprot->writeString((*_iter17));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Vendor) {
    xfer += oprot->writeFieldBegin("Vendor", ::apache::thrift::protocol::T_STRING, 8);
    xfer += oprot->writeString(this->Vendor);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.VendorDomain) {
    xfer += oprot->writeFieldBegin("VendorDomain", ::apache::thrift::protocol::T_STRING, 9);
    xfer += oprot->writeString(this->VendorDomain);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.ServiceUrl) {
    xfer += oprot->writeFieldBegin("ServiceUrl", ::apache::thrift::protocol::T_STRING, 10);
    xfer += oprot->writeString(this->ServiceUrl);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.UpdateUrl) {
    xfer += oprot->writeFieldBegin("UpdateUrl", ::apache::thrift::protocol::T_STRING, 11);
    xfer += oprot->writeString(this->UpdateUrl);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MExecutableDescription &a, MExecutableDescription &b) {
  using ::std::swap;
  swap(a.Name, b.Name);
  swap(a.ID, b.ID);
  swap(a.Language, b.Language);
  swap(a.ExecutableName, b.ExecutableName);
  swap(a.Author, b.Author);
  swap(a.Version, b.Version);
  swap(a.Dependencies, b.Dependencies);
  swap(a.Vendor, b.Vendor);
  swap(a.VendorDomain, b.VendorDomain);
  swap(a.ServiceUrl, b.ServiceUrl);
  swap(a.UpdateUrl, b.UpdateUrl);
  swap(a.__isset, b.__isset);
}

MExecutableDescription::MExecutableDescription(const MExecutableDescription& other18) {
  Name = other18.Name;
  ID = other18.ID;
  Language = other18.Language;
  ExecutableName = other18.ExecutableName;
  Author = other18.Author;
  Version = other18.Version;
  Dependencies = other18.Dependencies;
  Vendor = other18.Vendor;
  VendorDomain = other18.VendorDomain;
  ServiceUrl = other18.ServiceUrl;
  UpdateUrl = other18.UpdateUrl;
  __isset = other18.__isset;
}
MExecutableDescription& MExecutableDescription::operator=(const MExecutableDescription& other19) {
  Name = other19.Name;
  ID = other19.ID;
  Language = other19.Language;
  ExecutableName = other19.ExecutableName;
  Author = other19.Author;
  Version = other19.Version;
  Dependencies = other19.Dependencies;
  Vendor = other19.Vendor;
  VendorDomain = other19.VendorDomain;
  ServiceUrl = other19.ServiceUrl;
  UpdateUrl = other19.UpdateUrl;
  __isset = other19.__isset;
  return *this;
}
void MExecutableDescription::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MExecutableDescription(";
  out << "Name=" << to_string(Name);
  out << ", " << "ID=" << to_string(ID);
  out << ", " << "Language=" << to_string(Language);
  out << ", " << "ExecutableName=" << to_string(ExecutableName);
  out << ", " << "Author=" << to_string(Author);
  out << ", " << "Version=" << to_string(Version);
  out << ", " << "Dependencies="; (__isset.Dependencies ? (out << to_string(Dependencies)) : (out << "<null>"));
  out << ", " << "Vendor="; (__isset.Vendor ? (out << to_string(Vendor)) : (out << "<null>"));
  out << ", " << "VendorDomain="; (__isset.VendorDomain ? (out << to_string(VendorDomain)) : (out << "<null>"));
  out << ", " << "ServiceUrl="; (__isset.ServiceUrl ? (out << to_string(ServiceUrl)) : (out << "<null>"));
  out << ", " << "UpdateUrl="; (__isset.UpdateUrl ? (out << to_string(UpdateUrl)) : (out << "<null>"));
  out << ")";
}


MServiceDescription::~MServiceDescription() noexcept {
}


void MServiceDescription::__set_Name(const std::string& val) {
  this->Name = val;
}

void MServiceDescription::__set_ID(const std::string& val) {
  this->ID = val;
}

void MServiceDescription::__set_Language(const std::string& val) {
  this->Language = val;
}

void MServiceDescription::__set_Addresses(const std::vector<MIPAddress> & val) {
  this->Addresses = val;
}

void MServiceDescription::__set_Properties(const std::map<std::string, std::string> & val) {
  this->Properties = val;
__isset.Properties = true;
}

void MServiceDescription::__set_Parameters(const std::vector<MParameter> & val) {
  this->Parameters = val;
__isset.Parameters = true;
}
std::ostream& operator<<(std::ostream& out, const MServiceDescription& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MServiceDescription::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Name = false;
  bool isset_ID = false;
  bool isset_Language = false;
  bool isset_Addresses = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          isset_Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ID);
          isset_ID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Language);
          isset_Language = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Addresses.clear();
            uint32_t _size20;
            ::apache::thrift::protocol::TType _etype23;
            xfer += iprot->readListBegin(_etype23, _size20);
            this->Addresses.resize(_size20);
            uint32_t _i24;
            for (_i24 = 0; _i24 < _size20; ++_i24)
            {
              xfer += this->Addresses[_i24].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_Addresses = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->Properties.clear();
            uint32_t _size25;
            ::apache::thrift::protocol::TType _ktype26;
            ::apache::thrift::protocol::TType _vtype27;
            xfer += iprot->readMapBegin(_ktype26, _vtype27, _size25);
            uint32_t _i29;
            for (_i29 = 0; _i29 < _size25; ++_i29)
            {
              std::string _key30;
              xfer += iprot->readString(_key30);
              std::string& _val31 = this->Properties[_key30];
              xfer += iprot->readString(_val31);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.Properties = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Parameters.clear();
            uint32_t _size32;
            ::apache::thrift::protocol::TType _etype35;
            xfer += iprot->readListBegin(_etype35, _size32);
            this->Parameters.resize(_size32);
            uint32_t _i36;
            for (_i36 = 0; _i36 < _size32; ++_i36)
            {
              xfer += this->Parameters[_i36].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.Parameters = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_Name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ID)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Language)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_Addresses)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MServiceDescription::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MServiceDescription");

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ID", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->ID);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Language", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->Language);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Addresses", ::apache::thrift::protocol::T_LIST, 4);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->Addresses.size()));
    std::vector<MIPAddress> ::const_iterator _iter37;
    for (_iter37 = this->Addresses.begin(); _iter37 != this->Addresses.end(); ++_iter37)
    {
      xfer += (*_iter37).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  if (this->__isset.Properties) {
    xfer += oprot->writeFieldBegin("Properties", ::apache::thrift::protocol::T_MAP, 5);
    {
      xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_STRING, ::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->Properties.size()));
      std::map<std::string, std::string> ::const_iterator _iter38;
      for (_iter38 = this->Properties.begin(); _iter38 != this->Properties.end(); ++_iter38)
      {
        xfer += oprot->writeString(_iter38->first);
        xfer += oprot->writeString(_iter38->second);
      }
      xfer += oprot->writeMapEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.Parameters) {
    xfer += oprot->writeFieldBegin("Parameters", ::apache::thrift::protocol::T_LIST, 6);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->Parameters.size()));
      std::vector<MParameter> ::const_iterator _iter39;
      for (_iter39 = this->Parameters.begin(); _iter39 != this->Parameters.end(); ++_iter39)
      {
        xfer += (*_iter39).write(oprot);
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MServiceDescription &a, MServiceDescription &b) {
  using ::std::swap;
  swap(a.Name, b.Name);
  swap(a.ID, b.ID);
  swap(a.Language, b.Language);
  swap(a.Addresses, b.Addresses);
  swap(a.Properties, b.Properties);
  swap(a.Parameters, b.Parameters);
  swap(a.__isset, b.__isset);
}

MServiceDescription::MServiceDescription(const MServiceDescription& other40) {
  Name = other40.Name;
  ID = other40.ID;
  Language = other40.Language;
  Addresses = other40.Addresses;
  Properties = other40.Properties;
  Parameters = other40.Parameters;
  __isset = other40.__isset;
}
MServiceDescription& MServiceDescription::operator=(const MServiceDescription& other41) {
  Name = other41.Name;
  ID = other41.ID;
  Language = other41.Language;
  Addresses = other41.Addresses;
  Properties = other41.Properties;
  Parameters = other41.Parameters;
  __isset = other41.__isset;
  return *this;
}
void MServiceDescription::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MServiceDescription(";
  out << "Name=" << to_string(Name);
  out << ", " << "ID=" << to_string(ID);
  out << ", " << "Language=" << to_string(Language);
  out << ", " << "Addresses=" << to_string(Addresses);
  out << ", " << "Properties="; (__isset.Properties ? (out << to_string(Properties)) : (out << "<null>"));
  out << ", " << "Parameters="; (__isset.Parameters ? (out << to_string(Parameters)) : (out << "<null>"));
  out << ")";
}

} // namespace
