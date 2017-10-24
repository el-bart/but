#pragma once
#include <string>
#include <vector>
#include <iosfwd>
#include <cinttypes>

namespace But
{
namespace Log
{
namespace Backend
{

class Tag;
class Value;
class FieldInfo;

namespace detail
{

struct StreamVisitor final
{
  void operator()(bool b);
  void operator()(double fp);
  void operator()(std::string const& str);
  void operator()(int64_t n);
  void operator()(uint64_t n);

  void operator()(Backend::Tag const&, Backend::Value const& v);
  void operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo> const& fis);

  std::ostream* os_{nullptr};
};

}
}
}
}
