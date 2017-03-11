#include <boost/filesystem/operations.hpp>
#include "gtest/gtest.h"
#include "TempFile.hpp"

namespace fs = boost::filesystem;
using But::System::TempFile;

namespace
{

struct ButSystemTempFile: public testing::Test
{
  const boost::filesystem::path nameTemplate_{"/tmp/but_test_temporary_file"};
};


TEST_F(ButSystemTempFile, FileIsCreated)
{
  const TempFile tmp{nameTemplate_};
  EXPECT_TRUE( fs::exists( tmp.path() ) );

  fs::remove( tmp.path() );
}


TEST_F(ButSystemTempFile, FileHasProperPermissions)
{
  const TempFile tmp{nameTemplate_};
  EXPECT_EQ( 0600, fs::status( tmp.path() ).permissions() );

  fs::remove( tmp.path() );
}


TEST_F(ButSystemTempFile, UnlinkingFile)
{
  TempFile tmp{nameTemplate_};
  EXPECT_TRUE( fs::exists( tmp.path() ) );
  EXPECT_TRUE( tmp.descriptor().opened() );
  tmp.unlink();
  EXPECT_FALSE( fs::exists( tmp.path() ) );
  EXPECT_TRUE( tmp.descriptor().opened() );

  fs::remove( tmp.path() );
}


TEST_F(ButSystemTempFile, PathMathesTemplate)
{
  const TempFile tmp{nameTemplate_};
  fs::remove( tmp.path() );

  auto p = tmp.path().string();
  ASSERT_EQ( nameTemplate_.string().size() + 1 + 9, p.size() );
  p = p.substr( 0, p.size()-10 );
  EXPECT_EQ( nameTemplate_.string(), p );
}

}
