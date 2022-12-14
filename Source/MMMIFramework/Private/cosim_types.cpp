/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "cosim_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace MMIStandard {


MCoSimulationEvents::~MCoSimulationEvents() noexcept {
}


void MCoSimulationEvents::__set_Events(const std::vector< ::MMIStandard::MSimulationEvent> & val) {
  this->Events = val;
}

void MCoSimulationEvents::__set_SimulationTime(const double val) {
  this->SimulationTime = val;
}

void MCoSimulationEvents::__set_FrameNumber(const int32_t val) {
  this->FrameNumber = val;
}
std::ostream& operator<<(std::ostream& out, const MCoSimulationEvents& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t MCoSimulationEvents::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_Events = false;
  bool isset_SimulationTime = false;
  bool isset_FrameNumber = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Events.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->Events.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->Events[_i4].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_Events = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->SimulationTime);
          isset_SimulationTime = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->FrameNumber);
          isset_FrameNumber = true;
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

  if (!isset_Events)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_SimulationTime)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_FrameNumber)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t MCoSimulationEvents::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MCoSimulationEvents");

  xfer += oprot->writeFieldBegin("Events", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->Events.size()));
    std::vector< ::MMIStandard::MSimulationEvent> ::const_iterator _iter5;
    for (_iter5 = this->Events.begin(); _iter5 != this->Events.end(); ++_iter5)
    {
      xfer += (*_iter5).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("SimulationTime", ::apache::thrift::protocol::T_DOUBLE, 2);
  xfer += oprot->writeDouble(this->SimulationTime);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("FrameNumber", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->FrameNumber);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MCoSimulationEvents &a, MCoSimulationEvents &b) {
  using ::std::swap;
  swap(a.Events, b.Events);
  swap(a.SimulationTime, b.SimulationTime);
  swap(a.FrameNumber, b.FrameNumber);
}

MCoSimulationEvents::MCoSimulationEvents(const MCoSimulationEvents& other6) {
  Events = other6.Events;
  SimulationTime = other6.SimulationTime;
  FrameNumber = other6.FrameNumber;
}
MCoSimulationEvents& MCoSimulationEvents::operator=(const MCoSimulationEvents& other7) {
  Events = other7.Events;
  SimulationTime = other7.SimulationTime;
  FrameNumber = other7.FrameNumber;
  return *this;
}
void MCoSimulationEvents::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MCoSimulationEvents(";
  out << "Events=" << to_string(Events);
  out << ", " << "SimulationTime=" << to_string(SimulationTime);
  out << ", " << "FrameNumber=" << to_string(FrameNumber);
  out << ")";
}

} // namespace