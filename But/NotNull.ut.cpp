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
};

struct Derived: public Base
{
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
  EXPECT_THROW( SharedNN{std::unique_ptr<Data>{}}, SharedNN::NullPointer );
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


TEST_F(ButNotNull, BuildingUniquePtrFromRaw)
{
  UniqueNN p{new Data{"ok"}};
  EXPECT_EQ("ok", p->s_);
}


TEST_F(ButNotNull, BuildingSharedPtrFromRaw)
{
  SharedNN p{new Data{"ok"}};
  EXPECT_EQ("ok", p->s_);
}


TEST_F(ButNotNull, BuildingSharedPtrFromUniquePtr)
{
  SharedNN p1{ std::make_unique<Data>("ok") };
  EXPECT_EQ("ok", p1->s_);

  UniqueNN u1{ std::make_unique<Data>("ok") };
  SharedNN p2{ std::move(u1) };
  EXPECT_EQ("ok", p2->s_);
}


TEST_F(ButNotNull, ConvertingFromDerivedToBase)
{
  Derived                           derived;
  NotNull<Derived*>                 rd{&derived};
  NotNull<std::unique_ptr<Derived>> ud{ std::make_unique<Derived>() };
  NotNull<std::shared_ptr<Derived>> sd{ std::make_shared<Derived>() };

  NotNull<Base*>                 rb{rd};
  NotNull<std::unique_ptr<Base>> ub{ std::move(ud) };
  NotNull<std::shared_ptr<Base>> sb{sd};
}


TEST_F(ButNotNull, Copying)
{
  RawNN    r = r_;
  EXPECT_FALSE( std::is_copy_constructible<UniqueNN>::value );
  SharedNN s = s_;
  EXPECT_TRUE( r.get() == r_.get() );
  EXPECT_TRUE( s.get() == s_.get() );
}


TEST_F(ButNotNull, Moving)
{
  RawNN    r = std::move(r_);
  UniqueNN u = std::move(u_);
  SharedNN s = std::move(s_);
  EXPECT_EQ( "raw",    r->s_ );
  EXPECT_EQ( "unique", u->s_ );
  EXPECT_EQ( "shared", s->s_ );
}


TEST_F(ButNotNull, ConvertingToBool)
{
  if(r_) { } else FAIL() << "raw pointer";
  if(u_) { } else FAIL() << "unique pointer";
  if(s_) { } else FAIL() << "shared pointer";
}


TEST_F(ButNotNull, ConvertingToNotBool)
{
  if(not r_) FAIL() << "raw pointer";
  if(not u_) FAIL() << "unique pointer";
  if(not s_) FAIL() << "shared pointer";
}


TEST_F(ButNotNull, CopyInpternalPointerType)
{
  Data*                 p1 = r_.pointer();
  std::shared_ptr<Data> p3 = s_.pointer();
  ASSERT_TRUE(p1);
  ASSERT_TRUE(p3.get());
}


TEST_F(ButNotNull, MoveInpternalPointerType)
{
  Data*                 p1 = std::move(r_).pointerMove();
  std::unique_ptr<Data> p2 = std::move(u_).pointerMove();
  std::shared_ptr<Data> p3 = std::move(s_).pointerMove();
  ASSERT_TRUE(p1);
  ASSERT_TRUE(p2.get());
  ASSERT_TRUE(p3.get());
}


TEST_F(ButNotNull, Swap)
{
  UniqueNN p{ std::make_unique<Data>("new one") };
  std::swap(p, u_);
  EXPECT_EQ("unique",  p->s_);
  EXPECT_EQ("new one", u_->s_);
}


TEST_F(ButNotNull, LessThanOperator)
{
  UniqueNN p{ std::make_unique<Data>("new one") };
  if( p.get() >= u_.get() )
    std::swap(p, u_);
  EXPECT_TRUE( p < u_ );
}


TEST_F(ButNotNull, DerivedComparisonOperators)
{
  UniqueNN p{ std::make_unique<Data>("new one") };
  if( p.get() > u_.get() )
    std::swap(p, u_);

  EXPECT_FALSE( p >  u_ );
  EXPECT_FALSE( p >= u_ );
  EXPECT_TRUE ( p <= u_ );
  EXPECT_FALSE( p == u_ );
  EXPECT_TRUE ( p != u_ );

  EXPECT_TRUE ( u_ >  p  );
  EXPECT_TRUE ( u_ >= p  );
  EXPECT_FALSE( u_ <= p  );
  EXPECT_TRUE ( u_ == u_ );
  EXPECT_FALSE( u_ != u_ );
}

}
