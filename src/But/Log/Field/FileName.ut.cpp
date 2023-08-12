#include <But/Log/Field/FileName.hpp>
#include <But/Log/Backend/EntryRootTestBase.ut.hpp>

using But::Log::Field::FileName;

namespace
{

struct ButLogFieldFileName: public But::Log::Backend::EntryRootTestBase
{
  const FileName fn_{"some/file/path.txt"};
};


TEST_F(ButLogFieldFileName, NameAndValue)
{
  EXPECT_EQ( fieldName(&fn_), "But::FileName" );
  EXPECT_EQ( fieldValue(fn_), std::string{"some/file/path.txt"} );
}


TEST_F(ButLogFieldFileName, Logging)
{
  er_.proxy().nest(fn_);
  EXPECT_EQ_JSON(R"({ "But::FileName": "some/file/path.txt" })", er_);
}

}
