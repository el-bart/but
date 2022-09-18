#include <But/Format/format.hpp>
#include <But/Log/Localization/Translation.hpp>

namespace But
{
namespace Log
{
namespace Localization
{

namespace
{
auto makeTranslation(Translation::Data::value_type const& data)
{
  const auto input = BUT_FORMAT_RUNTIME(data.from_.format_);
  auto translation = BUT_FORMAT_RUNTIME(data.to_.format_);
  if( translation.expectedArguments() != input.expectedArguments() )
    BUT_THROW(Translation::IncompatibleFormats, "input format (\"" << input.inputFormat() << "\") requires " << input.expectedArguments()
        << " argument(s), while translated format (\"" << translation.inputFormat() << "\") requires "
        << translation.expectedArguments() << " argument(s)" );
  return translation;
}

template<typename Map>
auto make(Translation::Data&& data)
{
  Map m;
  for(auto&& e: data)
  {
    auto translation = makeTranslation(e);
    m.insert( typename Map::value_type{ std::move(e.from_.format_), std::move(translation) } );
  }
  return m;
}
}

Translation::Translation(Data data):
  translations_{ makeSharedNN<const Map>( make<Map>( std::move(data) ) ) }
{ }


Format::ParsedRuntime Translation::findTranslation(char const* from) const
{
  const auto it = translations_->find(from);
  if( it == translations_->end() )
    return Format::ParsedRuntime{from};
  return it->second;
}

}
}
}
