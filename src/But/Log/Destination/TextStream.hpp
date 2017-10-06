#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include "detail/StreamAndTrimVisitor.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class TextStream: public Foreign
{
protected:
  explicit TextStream(std::ostream& os): os_{&os} { }

  std::string const& endline() const { return endline_; }

private:
  void logImpl(Backend::FieldInfo const& fi) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    toStreamFormat(*os_, fi);
  }

  void reloadImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    reloadImplUnderLock();
  }

  void flushImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    (*os_) << std::flush;
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
  std::ostream* os_;
  const std::string endline_{ endlineType() };
  const Backend::NonPrintableTrimmer trim_;
};

}
}
}
