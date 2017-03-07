#pragma once
#include <string>
#include "Exception.hpp"

namespace But
{
namespace Log
{

class Format final
{
public:
  BUT_DEFINE_EXCEPTION(Invalid, Exception, "invalid format");

  Format() = default;
  explicit Format(std::string fmt):
    fmt_{ std::move(fmt) }
  {
    validate();
  }

  std::string const& format() const { return fmt_; }

private:
  void validate() const;

  std::string fmt_;
};

}
}
