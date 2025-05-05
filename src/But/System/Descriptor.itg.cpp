#include <fstream>
#include <type_traits>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
#include <boost/lexical_cast.hpp>
#include <gtest/gtest.h>
#include <But/System/Descriptor.hpp>
#include <But/assert.hpp>

namespace fs = std::filesystem;
using But::System::Descriptor;

namespace
{

struct TestDescriptor
{
  TestDescriptor():
    fd_{ open("/etc/fstab", O_RDONLY) },
    fdProcPath_{ fs::path{"/proc"} / boost::lexical_cast<std::string>(getpid()) / "fdinfo" / boost::lexical_cast<std::string>(fd_) }
  {
    if(fd_ == -1)
      throw std::logic_error{"unable to create descriptor for tests"};
    if( not isOpen() )
      throw std::logic_error{"descriptor does not appear to be opened: " + fdProcPath_.string()};
  }

  ~TestDescriptor()
  {
    if(fd_!=-1)
      close(fd_);
  }

  int releaseFd()
  {
    BUT_ASSERT( isOpen() );
    auto tmp = fd_;
    fd_ = -1;
    return tmp;
  }

  bool isOpen()
  {
    return fs::exists(fdProcPath_);
  }

private:
  int            fd_;
  const fs::path fdProcPath_;
};



struct ButSystemDescriptor: public testing::Test
{
  TestDescriptor td_;
};


TEST_F(ButSystemDescriptor, TestSuiteSanityCheck)
{
  close( td_.releaseFd() );
  EXPECT_FALSE( td_.isOpen() );
}


TEST_F(ButSystemDescriptor, NoDescriptor)
{
  const Descriptor d;
  EXPECT_FALSE( d.opened() );
  EXPECT_EQ(-1, d.get());
}


TEST_F(ButSystemDescriptor, Closing)
{
  {
    const Descriptor d{ td_.releaseFd() };
    EXPECT_TRUE( td_.isOpen() );
  }
  EXPECT_FALSE( td_.isOpen() );
}


TEST_F(ButSystemDescriptor, Getting)
{
  const auto tmp = td_.releaseFd();
  Descriptor d{tmp};
  EXPECT_EQ(tmp, d.get());
  d.reset();
  EXPECT_EQ(-1, d.get());
}


TEST_F(ButSystemDescriptor, NonCopyable)
{
  EXPECT_FALSE( std::is_copy_assignable<Descriptor>::value );
  EXPECT_FALSE( std::is_copy_constructible<Descriptor>::value );
}


TEST_F(ButSystemDescriptor, Movable)
{
  Descriptor d1{ td_.releaseFd() };
  EXPECT_TRUE( td_.isOpen() );
  Descriptor d2{ std::move(d1) };
  EXPECT_TRUE( td_.isOpen() );
  EXPECT_FALSE( d1.opened() );
  EXPECT_TRUE( d2.opened() );
  d1 = std::move(d2);
  EXPECT_TRUE( td_.isOpen() );
  EXPECT_TRUE( d1.opened() );
  EXPECT_FALSE( d2.opened() );
}


TEST_F(ButSystemDescriptor, Releasing)
{
  int x = 0;
  {
    const auto tmp = td_.releaseFd();
    Descriptor d{tmp};
    EXPECT_TRUE( d.opened() );
    x = d.release();
    EXPECT_FALSE( d.opened() );
    EXPECT_EQ(tmp, x) << "invalid descriptor released";
  }
  EXPECT_TRUE( td_.isOpen() );
  close(x);
}


TEST_F(ButSystemDescriptor, Reseting)
{
  Descriptor d{ td_.releaseFd() };
  EXPECT_TRUE( d.opened() );
  EXPECT_TRUE( td_.isOpen() );
  d.reset();
  EXPECT_FALSE( d.opened() );
  EXPECT_FALSE( td_.isOpen() );
  d.reset();
  EXPECT_FALSE( d.opened() );
  EXPECT_FALSE( td_.isOpen() );
}


TEST_F(ButSystemDescriptor, ResetingToNewValue)
{
  TestDescriptor td2;
  {
    Descriptor d{ td_.releaseFd() };
    EXPECT_TRUE( d.opened() );
    EXPECT_TRUE( td_.isOpen() );
    d.reset( td2.releaseFd() );
    EXPECT_TRUE( d.opened() );
    EXPECT_FALSE( td_.isOpen() );
    EXPECT_TRUE( td2.isOpen() );
  }
  EXPECT_FALSE( td2.isOpen() );
}


TEST_F(ButSystemDescriptor, SwappingMemberFunction)
{
  TestDescriptor td1;
  TestDescriptor td2;
  {
    Descriptor d1{td1.releaseFd()};
    {
      Descriptor d2{td2.releaseFd()};
      const auto g1 = d1.get();
      const auto g2 = d2.get();
      d2.swap(d1);
      EXPECT_EQ(g2, d1.get());
      EXPECT_EQ(g1, d2.get());
    }
    EXPECT_FALSE( td1.isOpen() );
    EXPECT_TRUE( td2.isOpen() );
  }
  EXPECT_FALSE( td1.isOpen() );
  EXPECT_FALSE( td1.isOpen() );
}


TEST_F(ButSystemDescriptor, SwapFreeFunction)
{
  TestDescriptor td1;
  TestDescriptor td2;
  {
    Descriptor d1{td1.releaseFd()};
    {
      Descriptor d2{td2.releaseFd()};
      const auto g1 = d1.get();
      const auto g2 = d2.get();
      swap(d1, d2);
      EXPECT_EQ(g2, d1.get());
      EXPECT_EQ(g1, d2.get());
    }
    EXPECT_FALSE( td1.isOpen() );
    EXPECT_TRUE( td2.isOpen() );
  }
  EXPECT_FALSE( td1.isOpen() );
  EXPECT_FALSE( td1.isOpen() );
}


TEST_F(ButSystemDescriptor, ExplicitToBoolConversion)
{
  Descriptor fd{ td_.releaseFd() };
  auto const& cref = fd;
  EXPECT_TRUE( static_cast<bool>(cref) );
  fd.reset();
  EXPECT_FALSE( static_cast<bool>(cref) );
}

}
