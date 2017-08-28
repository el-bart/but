#include "Translation.hpp"

namespace But
{
namespace Log
{
namespace Localization
{

namespace
{
auto makeMap(Translation::Data&& data)
{
  std::unordered_map<Translation::Entry::From, Translation::Entry::To> out;
  (void)data;       // TODO....             
  return out;
}
}

Translation::Translation(Data data):
  translations_{ makeSharedNN<Translations>( makeMap( std::move(data) ) ) }
{
  (void)data;
  throw 2;  // TODO...              
}


char const* Translation::findTranslation(char const* from) const
{
  (void)from;
  throw 1;  // TODO...              
}

}
}
}
