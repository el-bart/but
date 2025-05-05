#include <gtest/gtest.h>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Backend/EntryRootTestBase.utg.hpp>

using But::Log::Field::LineNumber;

namespace
{

struct ButLogFieldLineNumber: public But::Log::Backend::EntryRootTestBase
{
  const LineNumber ln_{666};
};


TEST_F(ButLogFieldLineNumber, NameAndValue)
{
  EXPECT_EQ( fieldName(&ln_), "But::LineNumber" );
  EXPECT_EQ( fieldValue(ln_), 666 );
}


TEST_F(ButLogFieldLineNumber, Logging)
{
  er_.proxy().nest(ln_);
  EXPECT_EQ_JSON(R"({ "But::LineNumber": 666 })", er_);
}

}
