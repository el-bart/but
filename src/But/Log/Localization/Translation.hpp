#pragma once
#include <vector>
#include <unordered_map>
#include "But/NotNull.hpp"
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

  using Translations = std::unordered_map<Entry::From, Entry::To>;
  using TranslationShPtr = But::NotNullShared<const Translations>;
  TranslationShPtr translations_;
};

}
}
}


namespace std
{
template<>
struct hash<But::Log::Localization::Translation::Entry::From>
{
  auto operator()(But::Log::Localization::Translation::Entry::From const& in) const
  {
    return hash_(in.format_);
  }

private:
  const std::hash<std::string> hash_;
};
}
