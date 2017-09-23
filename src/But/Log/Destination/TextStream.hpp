#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include "But/Log/Backend/toValue.hpp"
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
  void logImpl(Backend::Entry const& e) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    toStreamFormat(*os_, e);
  }

  void logImpl(Field::FormattedString const& str, Backend::Entry const& e) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    toStreamFormat(*os_, str, e);
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

  virtual void toStreamFormat(std::ostream& os, Backend::Entry const& e)
  {
    if( not e.empty() )
    {
      auto it = begin(e);
      detail::StreamAndTrimVisitor satv{&trim_, &os};
      it->value().visit(satv);
      for(++it; it!=end(e); ++it)
      {
        os << ' ';
        it->value().visit(satv);
      }
    }
    os << endline();
  }

  virtual void toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const&)
  {
    detail::StreamAndTrimVisitor satv{&trim_, &os};
    satv(str.value_);
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
