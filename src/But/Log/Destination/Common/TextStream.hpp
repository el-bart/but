#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include <But/Log/Destination/detail/StreamAndTrimVisitor.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

class TextStream: public Sink
{
protected:
  explicit TextStream(std::ostream& os): os_{os} { }

  std::string const& endline() const { return endline_; }
  auto const& trim() const { return trim_; }

private:
  void logImpl(Backend::FieldInfo const& fi) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    toStreamFormat(os_, fi);
  }

  void reloadImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    reloadImplUnderLock();
  }

  void flushImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    os_ << std::flush;
  }

  virtual void reloadImplUnderLock() = 0;

  virtual void toStreamFormat(std::ostream& os, Backend::FieldInfo const& fi)
  {
    detail::StreamAndTrimVisitor satv{&trim_, &os};
    fi.visit(satv);
    os << endline();
  }

  static std::string endlineType()
  {
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
  }

  std::mutex mutex_;
  std::ostream& os_;
  const std::string endline_{ endlineType() };
  const Backend::NonPrintableTrimmer trim_;
};

}
}
}
}
