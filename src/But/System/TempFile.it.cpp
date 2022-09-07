#include <filesystem>
#include <gtest/gtest.h>
#include <But/System/TempFile.hpp>

namespace fs = std::filesystem;
using But::System::TempFile;

namespace
{

struct ButSystemTempFile: public testing::Test
{
  const fs::path nameTemplate_{"/tmp/but_test_temporary_file"};
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
  using std::filesystem::perms;
  EXPECT_EQ( perms::owner_read | perms::owner_write, fs::status( tmp.path() ).permissions() );
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
  ASSERT_EQ( nameTemplate_.string().size() + 1 + 6, p.size() );
  p = p.substr( 0, p.size()-1-6 );
  EXPECT_EQ( nameTemplate_.string(), p );
}

}
