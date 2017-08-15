#include "TextFile.hpp"

namespace But
{
namespace Log
{
namespace Destination
{


TextFile::TextFile(boost::filesystem::path path):
  Stream{file_},                // NOTE: not really used until the object is fully-constructed
  path_{ std::move(path) }
{
  reload();
}


void TextFile::reloadImplUnderLock()
{
  std::ofstream reloaded;
  reloaded.open( path_.string() );
  if( not reloaded.is_open() )
    BUT_THROW(OpeningLogFileFailed, path_);

  file_.close();
  std::swap(file_, reloaded);
}

}
}
}
