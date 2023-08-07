#include <filesystem>
#include <But/Log/Destination/File.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

File::File(std::filesystem::path path):
  Common::LockedStream{file_},        // NOTE: not really used until the object is fully-constructed
  path_{ std::move(path) }
{
  File::reloadImplUnderLock();
}


namespace
{
void createFileIfMissing(std::filesystem::path const& path)
{
  if( std::filesystem::exists(path) )
    return;
  constexpr auto mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
  std::fstream file{ path.string(), mode };
  if( not file )
    BUT_THROW(File::OpeningLogFileFailed, "failed to create file: " << path);
}
}

void File::reloadImplUnderLock()
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
