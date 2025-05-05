#include <gtest/gtest.h>
#include <But/Log/Destination/Null.hpp>

using But::Log::Destination::Null;
using But::Log::Destination::Sink;

namespace
{

struct ButLogDestinationNull: public testing::Test
{
  Null null_;
};


TEST_F(ButLogDestinationNull, SmokeTest)
{
  null_.reload();
  null_.flush();
  null_.log("ignored");
}


TEST_F(ButLogDestinationNull, BaseClassSmokeTest)
{
  Sink& base_{null_};
  base_.reload();
  base_.flush();
  base_.log("ignored");
}

}
