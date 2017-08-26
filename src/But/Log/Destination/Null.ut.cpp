#include "gtest/gtest.h"
#include "Null.hpp"

using But::Log::Destination::Null;
using But::Log::Destination::Foreign;
using But::Log::Field::FormattedString;

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
  null_.log( FormattedString{"$1 $0"}, 42, "answer is" );
}


TEST_F(ButLogDestinationNull, BaseClassSmokeTest)
{
  Foreign& base_{null_};
  base_.reload();
  base_.flush();
  base_.log("some", "values", 42);
}

}
