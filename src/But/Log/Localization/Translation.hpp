#pragma once
#include <string>
#include <vector>
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

  template<unsigned N, unsigned M>
  auto translate(Format::Parsed<N,M>&& parsed) const
  {
    const auto translation = findTranslation( parsed.inputFormat() );
    if(not translation)
      return std::move(parsed);
    // TODO...
    return Format::Parsed<N,M>(translation);
  }

private:
  char const* findTranslation(char const* from) const;

  using DataShNN = But::NotNullShared<const Data>;
  DataShNN data_;
};


bool operator<(Translation::Entry const& lhs, Translation::Entry const& rhs);
bool operator<(char const* lhs,               Translation::Entry const& rhs);
bool operator<(Translation::Entry const& lhs, char const* rhs);

}
}
}
