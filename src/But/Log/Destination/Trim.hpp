#pragma once
#include <But/NotNull.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Backend/NonPrintableTrimmer.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

struct Trim final: public Sink
{
  explicit Trim(NotNullShared<Sink> sink):
    sink_{ std::move(sink) }
  { }

private:
  void logImpl(std::string&& str) override final { sink_->log( trim_( std::move(str) ) ); }
  void reloadImpl()               override final { sink_->reload(); }
  void flushImpl()                override final { sink_->flush(); }

  const Backend::NonPrintableTrimmer trim_;
  NotNullShared<Sink> sink_;
};

}
}
}
