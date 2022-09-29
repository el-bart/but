#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::FormattedString;

namespace
{

struct ButLogFieldFormattedString: public But::Log::Backend::detail::EntryRootTestBase
{
  const FormattedString fs_{"foo bar"};
};


TEST_F(ButLogFieldFormattedString, NameAndValue)
{
  EXPECT_EQ( fieldName(&fs_), "But::FormattedString" );
  EXPECT_EQ( fieldValue(fs_), "foo bar" );
}


TEST_F(ButLogFieldFormattedString, Logging)
{
  er_.proxy().nest(fs_);
  EXPECT_EQ_JSON(R"({ "But::FormattedString": "foo bar" })", er_);
}

}
