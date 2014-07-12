#include <memory>
#include <type_traits>

#include "gtest/gtest.h"
#include "NotNull.hpp"

struct Data
{
  explicit Data(std::string s = ""): s_{std::move(s)} { }
  std::string s_;
};

struct Base
{
  virtual ~Base() = default;
  std::string b_;
};

struct Derived: public Base
{
  std::string d_;
};


using But::NotNull;

using RawNN    = NotNull<Data*>;
using UniqueNN = NotNull<std::unique_ptr<Data>>;
using SharedNN = NotNull<std::shared_ptr<Data>>;

namespace
{

struct ButNotNull: public testing::Test
{
  Data     data_{"raw"};
  RawNN    r_{&data_};
  UniqueNN u_{ std::make_unique<Data>("unique") };
  SharedNN s_{ std::make_shared<Data>("shared") };
};


TEST_F(ButNotNull, NoDefaultConstructor)
{
  EXPECT_FALSE( std::is_default_constructible<RawNN>::value );
  EXPECT_FALSE( std::is_default_constructible<UniqueNN>::value );
  EXPECT_FALSE( std::is_default_constructible<SharedNN>::value );
}


TEST_F(ButNotNull, ExplicitConstructionFromNull)
{
  EXPECT_THROW( RawNN{nullptr},    RawNN::NullPointer    );
  EXPECT_THROW( UniqueNN{nullptr}, UniqueNN::NullPointer );
  EXPECT_THROW( SharedNN{nullptr}, SharedNN::NullPointer );
}


TEST_F(ButNotNull, AccessingWithConstGet)
{
  EXPECT_EQ("raw",    r_.get()->s_);
  EXPECT_EQ("unique", u_.get()->s_);
  EXPECT_EQ("shared", s_.get()->s_);
}


TEST_F(ButNotNull, ModyfingWithGet)
{
  r_.get()->s_ = "1";
  u_.get()->s_ = "2";
  s_.get()->s_ = "3";
  EXPECT_EQ("1", r_.get()->s_);
  EXPECT_EQ("2", u_.get()->s_);
  EXPECT_EQ("3", s_.get()->s_);
}


TEST_F(ButNotNull, ModyfingWithArrowOperator)
{
  r_->s_ = "1";
  u_->s_ = "2";
  s_->s_ = "3";
  EXPECT_EQ("1", r_->s_);
  EXPECT_EQ("2", u_->s_);
  EXPECT_EQ("3", s_->s_);
}


TEST_F(ButNotNull, PreservingConstness)
{
  Data                                 data;
  NotNull<Data const*>                 r{&data};
  NotNull<std::unique_ptr<const Data>> u{ std::make_unique<const Data>() };
  NotNull<std::shared_ptr<const Data>> s{ std::make_shared<const Data>() };
  EXPECT_TRUE( (std::is_same<Data const*, decltype(r.get())>::value) );
  EXPECT_TRUE( (std::is_same<Data const*, decltype(u.get())>::value) );
  EXPECT_TRUE( (std::is_same<Data const*, decltype(s.get())>::value) );
}


TEST_F(ButNotNull, ConvertingNonConstToConst)
{
  NotNull<Data const*>                 r{r_};
  NotNull<std::unique_ptr<const Data>> u{std::move(u_)};
  NotNull<std::shared_ptr<const Data>> s{s_};
  EXPECT_TRUE( (std::is_same<Data const*, decltype(r.get())>::value) );
  EXPECT_TRUE( (std::is_same<Data const*, decltype(u.get())>::value) );
  EXPECT_TRUE( (std::is_same<Data const*, decltype(s.get())>::value) );
}

}
