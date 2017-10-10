#pragma once
#include <string>
#include <unordered_map>
#include "But/NotNull.hpp"
#include "But/Exception.hpp"
#include "But/Format/Parsed.hpp"

namespace But
{
namespace Log
{
namespace Localization
{

/** @brief locationzation object providing localization service for all defined texts. formats that were not
 *         translated, will be left as they were.
 */
class Translation final
{
public:
  BUT_DEFINE_EXCEPTION(IncompatibleFormats, Exception, "incompatible formats");

  struct Entry final
  {
    struct From final { std::string format_; };
    struct To   final { std::string format_; };

    From from_;
    To   to_;
  };

  using Data = std::vector<Entry>;

  explicit Translation(Data data);

  template<size_t N, size_t M>
  auto translate(Format::ParsedCompiletime<N,M> const& parsed) const
  {
    return findTranslation( parsed.inputFormat() );
  }

private:
  using Map = std::unordered_map<std::string, Format::ParsedRuntime>;

  Format::ParsedRuntime findTranslation(char const* from) const;

  But::NotNullShared<const Map> translations_;
};


bool operator<(Translation::Entry const& lhs, Translation::Entry const& rhs);
bool operator<(char const* lhs,               Translation::Entry const& rhs);
bool operator<(Translation::Entry const& lhs, char const* rhs);

}
}
}
