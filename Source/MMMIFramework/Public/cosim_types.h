/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef cosim_TYPES_H
#define cosim_TYPES_H

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
#include "mmu_types.h"
#include "services_types.h"


namespace MMIStandard {

class MCoSimulationEvents;


class MMIFRAMEWORK_API MCoSimulationEvents : public virtual ::apache::thrift::TBase {
 public:

  MCoSimulationEvents(const MCoSimulationEvents&);
  MCoSimulationEvents& operator=(const MCoSimulationEvents&);
  MCoSimulationEvents() : SimulationTime(0), FrameNumber(0) {
  }

  virtual ~MCoSimulationEvents() noexcept;
  std::vector< ::MMIStandard::MSimulationEvent>  Events;
  double SimulationTime;
  int32_t FrameNumber;

  void __set_Events(const std::vector< ::MMIStandard::MSimulationEvent> & val);

  void __set_SimulationTime(const double val);

  void __set_FrameNumber(const int32_t val);

  bool operator == (const MCoSimulationEvents & rhs) const
  {
    if (!(Events == rhs.Events))
      return false;
    if (!(SimulationTime == rhs.SimulationTime))
      return false;
    if (!(FrameNumber == rhs.FrameNumber))
      return false;
    return true;
  }
  bool operator != (const MCoSimulationEvents &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MCoSimulationEvents & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MCoSimulationEvents &a, MCoSimulationEvents &b);

std::ostream& operator<<(std::ostream& out, const MCoSimulationEvents& obj);

} // namespace

#endif