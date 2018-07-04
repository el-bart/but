#include <cstring>
#include <boost/filesystem/operations.hpp>
#include <But/System/TempFile.hpp>

namespace But
{
namespace System
{

TempFile::File TempFile::initialize(boost::filesystem::path const& tmp)
{
  const auto& str = tmp.string();
  std::unique_ptr<char[]> tmpStr{ new char[ str.length() + 1 + 9 + 1 ] };
  sprintf( tmpStr.get(), "%s-XXXXXX", str.c_str() );
  Descriptor fd{ mkstemp( tmpStr.get() ) };
  if( not fd.opened() )
    BUT_THROW( CannotCreateTemporaryFile, "from " << tmp << ", failed: " << strerror(errno) );
  return File{ boost::filesystem::path{ tmpStr.get() }, std::move(fd) };
}


void TempFile::unlink()
{
  boost::filesystem::remove( path() );
}

}
}
