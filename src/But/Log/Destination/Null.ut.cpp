#include "gtest/gtest.h"
#include "Null.hpp"

using But::Log::Destination::Null;
using But::Log::Destination::Foreign;

namespace
{

struct ButLogDestinationNull: public testing::Test
{
  Null null_;
};


TEST_F(ButLogDestinationNull, ArrowOperatorSmokeTest)
{
  null_->reload();
}


TEST_F(ButLogDestinationNull, SmokeTest)
{
  null_.reload();
  null_.flush();
  null_.log("some", "values", 42);
}


TEST_F(ButLogDestinationNull, BaseClassSmokeTest)
{
  Foreign& base_{null_};
  base_.reload();
  base_.flush();
  base_.log("some", "values", 42);
}

}
