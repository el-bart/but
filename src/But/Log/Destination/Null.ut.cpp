#include <gtest/gtest.h>
#include <But/Log/Destination/Null.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;
using But::Log::Destination::Null;
using But::Log::Destination::Sink;
using But::Log::Field::FormattedString;

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
  null_.log( args2FieldInfo( "some", "values", 42 ) );
  null_.log( args2FieldInfo( FormattedString{"$1 $0"}, 42, "answer is" ) );
}


TEST_F(ButLogDestinationNull, BaseClassSmokeTest)
{
  Sink& base_{null_};
  base_.reload();
  base_.flush();
  base_.log( args2FieldInfo( "some", "values", 42 ) );
}

}
