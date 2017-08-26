#pragma once
#include "TextFile.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief outputs text file with 1 json log per line.
 */
class JsonFile final: public TextFile
{
public:
  explicit JsonFile(boost::filesystem::path path): TextFile{ std::move(path) } { }

private:
  void toStreamFormat(std::ostream& os, Backend::Entry const& e) override;
};

}
}
}
