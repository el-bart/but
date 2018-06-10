#pragma once
#include <But/Log/Destination/detail/VerboseStreamAndTrimVisitor.hpp>
#include <But/Log/Destination/Common/TextStream.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

class VerboseTextStream: public TextStream
{
protected:
  struct Depth
  {
    size_t value_;
  };

  VerboseTextStream(std::ostream& os, Depth depth): TextStream{os}, depth_{depth} { }

private:
  void toStreamFormat(std::ostream& os, Backend::FieldInfo const& fi) override
  {
    detail::VerboseStreamAndTrimVisitor satv{&trim(), &os, depth_.value_};
    fi.visit(satv);
    os << endline();
  }

  const Depth depth_;
};

}
}
}
}
