#pragma once
#include <boost/filesystem/path.hpp>
#include "Descriptor.hpp"
#include "FsError.hpp"

namespace But
{
namespace System
{

class TempFile final
{
public:
  BUT_DEFINE_EXCEPTION(CannotCreateTemporaryFile, FsError, "cannot crate temporary file");

  explicit TempFile(boost::filesystem::path const& nameTemplate):
    file_{ initialize(nameTemplate) }
  { }

  boost::filesystem::path const& path() const { return file_.path_; }
  Descriptor const& descriptor() const { return file_.fd_; }

  void unlink();

private:
  struct File
  {
    boost::filesystem::path path_;
    Descriptor fd_;
  };

  static File initialize(boost::filesystem::path const& tmp);

  File file_;
};

}
}
