#include "TextFile.hpp"

namespace But
{
namespace Log
{
namespace Destination
{


TextFile::TextFile(boost::filesystem::path path):
  path_{ std::move(path) },
  s_{file_}
{
  reload();
}


void TextFile::reloadImpl()
{
  std::ofstream reloaded;
  reloaded.open( path_.string() );
  if( not reloaded.is_open() )
    BUT_THROW(OpeningLogFileFailed, path_);

  file_.close();
  file_ = std::move(reloaded);

  s_.reload();
}

}
}
}
