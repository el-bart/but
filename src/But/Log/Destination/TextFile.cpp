#include <boost/filesystem/operations.hpp>
#include <But/Log/Destination/TextFile.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

TextFile::TextFile(boost::filesystem::path path):
  Common::TextStream{file_},        // NOTE: not really used until the object is fully-constructed
  path_{ std::move(path) }
{
  TextFile::reloadImplUnderLock();
}


namespace
{
void createFileIfMissing(boost::filesystem::path const& path)
{
  if( boost::filesystem::exists(path) )
    return;
  constexpr auto mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
  std::fstream file{ path.string(), mode };
  if( not file )
    BUT_THROW(TextFile::OpeningLogFileFailed, "failed to create file: " << path);
}
}

void TextFile::reloadImplUnderLock()
{
  createFileIfMissing(path_);
  std::ofstream reloaded;
  constexpr auto mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate;
  reloaded.open( path_.string(), mode );
  if( not reloaded.is_open() )
    BUT_THROW(OpeningLogFileFailed, path_);

  file_.close();
  std::swap(file_, reloaded);
}

}
}
}
