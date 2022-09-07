#pragma once
#include <filesystem>
#include <But/System/Descriptor.hpp>
#include <But/System/FsError.hpp>

namespace But
{
namespace System
{

class TempFile final
{
public:
  BUT_DEFINE_EXCEPTION(CannotCreateTemporaryFile, FsError, "cannot crate temporary file");

  explicit TempFile(std::filesystem::path const& nameTemplate):
    file_{ initialize(nameTemplate) }
  { }

  auto const& path() const { return file_.path_; }
  auto const& descriptor() const { return file_.fd_; }

  void unlink();

private:
  struct File
  {
    std::filesystem::path path_;
    Descriptor fd_;
  };

  static File initialize(std::filesystem::path const& tmp);

  File file_;
};

}
}
