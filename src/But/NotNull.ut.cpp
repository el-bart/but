#include <memory>
#include <string>
#include <type_traits>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <But/NotNull.hpp>

struct Data
{
  explicit Data(std::string s = ""): s_{std::move(s)} { }
  std::string s_;
};

struct Base
{
  Base(std::string s="base"): s_{std::move(s)} { }
  virtual ~Base() = default;

  std::string s_;
};

struct Derived: public Base
{
  Derived(): Base("derived") { }
};


using But::NotNull;
using But::makeNN;
using But::makeUniqueNN;
using But::makeSharedNN;

using RawNN    = NotNull<Data*>;
using UniqueNN = NotNull<std::unique_ptr<Data>>;
using SharedNN = NotNull<std::shared_ptr<Data>>;

using RawBaseNN    = NotNull<Base*>;
using UniqueBaseNN = NotNull<std::unique_ptr<Base>>;
using SharedBaseNN = NotNull<std::shared_ptr<Base>>;

using RawDerivedNN    = NotNull<Derived*>;
using UniqueDerivedNN = NotNull<std::unique_ptr<Derived>>;
using SharedDerivedNN = NotNull<std::shared_ptr<Derived>>;

namespace
{

struct ButNotNull: public testing::Test
{
  Data     data_{"raw"};
  RawNN    r_{&data_};
  UniqueNN u_{ std::make_unique<Data>("unique") };
  SharedNN s_{ std::make_shared<Data>("shared") };

  Derived         derived_;
  RawDerivedNN    rd_{&derived_};
  UniqueDerivedNN ud_{ std::make_unique<Derived>() };
  SharedDerivedNN sd_{ std::make_shared<Derived>() };

  Base         base_;
  RawBaseNN    rb_{&base_};
  UniqueBaseNN ub_{ std::make_unique<Base>() };
  SharedBaseNN sb_{ std::make_shared<Base>() };
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


TEST_F(ButNotNull, AssignemtFromNullPointer)
{
  Data*                 data = nullptr;
  std::unique_ptr<Data> unique;
  std::shared_ptr<Data> shared;
  EXPECT_THROW( r_ = data,              RawNN::NullPointer    );
  EXPECT_THROW( u_ = std::move(unique), UniqueNN::NullPointer );
  EXPECT_THROW( s_ = shared,            SharedNN::NullPointer );
  EXPECT_THROW( s_ = std::move(unique), SharedNN::NullPointer );

  EXPECT_EQ( "raw",    r_->s_ );
  EXPECT_EQ( "unique", u_->s_ );
  EXPECT_EQ( "shared", s_->s_ );
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
  NotNull<Base*>                 rb{rd_};
  NotNull<std::unique_ptr<Base>> ub{ std::move(ud_) };
  NotNull<std::shared_ptr<Base>> sb{sd_};
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


TEST_F(ButNotNull, CopyingFromDerived)
{
  RawBaseNN    r{rd_};
  EXPECT_FALSE( std::is_copy_constructible<UniqueBaseNN>::value );
  SharedBaseNN s{sd_};
  EXPECT_EQ( "derived", r->s_ );
  EXPECT_EQ( "derived", s->s_ );
}


TEST_F(ButNotNull, MovingFromDerived)
{
  RawBaseNN    r{ std::move(rd_) };
  UniqueBaseNN u{ std::move(ud_) };
  SharedBaseNN s{ std::move(sd_) };
  EXPECT_EQ( "derived", r->s_ );
  EXPECT_EQ( "derived", u->s_ );
  EXPECT_EQ( "derived", s->s_ );
}


TEST_F(ButNotNull, CopyingAssignemntFromDerived)
{
  rb_ = rd_;
  EXPECT_FALSE( std::is_copy_assignable<UniqueBaseNN>::value );
  sb_ = sd_;
  EXPECT_EQ( "derived", rb_->s_ );
  EXPECT_EQ( "derived", sb_->s_ );
}


TEST_F(ButNotNull, MovingAssignmentFromDerived)
{
  rb_ = std::move(rd_);
  ub_ = std::move(ud_);
  sb_ = std::move(sd_);
  EXPECT_EQ( "derived", rb_->s_ );
  EXPECT_EQ( "derived", ub_->s_ );
  EXPECT_EQ( "derived", sb_->s_ );
}


TEST_F(ButNotNull, CopyingAssignemntFromPointerType)
{
  rb_ = &base_;
  EXPECT_FALSE( std::is_copy_assignable<UniqueBaseNN>::value );
  const auto tmp = std::make_shared<Base>();
  sb_ = tmp;
  EXPECT_EQ( "base", rb_->s_ );
  EXPECT_EQ( "base", sb_->s_ );
}


TEST_F(ButNotNull, MovingAssignmentFromPointerType)
{
  rb_ = &base_;
  ub_ = std::make_unique<Base>();
  sb_ = std::make_shared<Base>();
  EXPECT_EQ( "base", rb_->s_ );
  EXPECT_EQ( "base", ub_->s_ );
  EXPECT_EQ( "base", sb_->s_ );
}


TEST_F(ButNotNull, CopyingAssignemntFromDerivedPointerType)
{
  rb_ = &derived_;
  EXPECT_FALSE( std::is_copy_assignable<UniqueBaseNN>::value );
  const auto tmp = std::make_shared<Derived>();
  sb_ = tmp;
  EXPECT_EQ( "derived", rb_->s_ );
  EXPECT_EQ( "derived", sb_->s_ );
}


TEST_F(ButNotNull, MovingAssignmentFromDerivedPointerType)
{
  rb_ = &derived_;
  ub_ = std::make_unique<Derived>();
  sb_ = std::make_shared<Derived>();
  EXPECT_EQ( "derived", rb_->s_ );
  EXPECT_EQ( "derived", ub_->s_ );
  EXPECT_EQ( "derived", sb_->s_ );

  sb_ = std::make_unique<Derived>();
  EXPECT_EQ( "derived", sb_->s_ );
}


TEST_F(ButNotNull, CopyingAssignmentFromSelf)
{
  rb_ = rb_;
  EXPECT_FALSE( std::is_copy_assignable<UniqueBaseNN>::value );
  sb_ = sb_;
  EXPECT_EQ( "base", rb_->s_ );
  EXPECT_EQ( "base", sb_->s_ );
}


TEST_F(ButNotNull, MovingAssignmentFromSelf)
{
  rb_ = std::move(rb_);
  ub_ = std::move(ub_);
  sb_ = std::move(sb_);
  EXPECT_EQ( "base", rb_->s_ );
  EXPECT_EQ( "base", ub_->s_ );
  EXPECT_EQ( "base", sb_->s_ );
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
  Data*                 p1 = r_.underlyingPointer();
  std::shared_ptr<Data> p3 = s_.underlyingPointer();
  ASSERT_TRUE(p1);
  ASSERT_TRUE(p3.get());
}


TEST_F(ButNotNull, MoveInpternalPointerType)
{
  Data*                 p1 = std::move(r_).underlyingPointer();
  std::unique_ptr<Data> p2 = std::move(u_).underlyingPointer();
  std::shared_ptr<Data> p3 = std::move(s_).underlyingPointer();
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


TEST_F(ButNotNull, Typedefs)
{
  EXPECT_TRUE( (std::is_same<Data, RawNN::element_type>::value) );
  EXPECT_TRUE( (std::is_same<Data, UniqueNN::element_type>::value) );
  EXPECT_TRUE( (std::is_same<Data, SharedNN::element_type>::value) );
}


TEST_F(ButNotNull, TypedefsForConstTypes)
{
  EXPECT_TRUE( (std::is_same<Data const, NotNull<Data const*>::element_type>::value) );
  EXPECT_TRUE( (std::is_same<Data const, NotNull<std::unique_ptr<Data const>>::element_type>::value) );
  EXPECT_TRUE( (std::is_same<Data const, NotNull<std::shared_ptr<Data const>>::element_type>::value) );
}


TEST_F(ButNotNull, BasicBoostCompatibility)
{
  using Ptr      = NotNull<boost::shared_ptr<Data>>;
  using PtrConst = NotNull<boost::shared_ptr<Data const>>;
  EXPECT_TRUE( (std::is_same<Data const, PtrConst::element_type>::value) );
  Ptr p{new Data};
  p->s_ = "foo";
  PtrConst pc{p};
  EXPECT_EQ("foo", pc->s_);
}


TEST_F(ButNotNull, DereferencingOperator)
{
  EXPECT_EQ("raw",    (*r_).s_);
  EXPECT_EQ("unique", (*u_).s_);
  EXPECT_EQ("shared", (*s_).s_);
}


TEST_F(ButNotNull, InplicitConversionToConst)
{
  using Ptr      = NotNull<boost::shared_ptr<Data>>;
  using PtrConst = NotNull<boost::shared_ptr<Data const>>;
  PtrConst pc{ Ptr{new Data} };
}


TEST_F(ButNotNull, MakeNN)
{
  auto ptr = makeNN<char const*>("stuff");
  EXPECT_TRUE( ( std::is_same<decltype(ptr), NotNull<char const*>>::value ) );
  ASSERT_TRUE(ptr);
  EXPECT_TRUE( ( strcmp(ptr.get(), "stuff") == 0 ) );
}


TEST_F(ButNotNull, MakeUniqueNN)
{
  auto ptr = makeUniqueNN<Data>("stuff");
  EXPECT_TRUE( ( std::is_same<decltype(ptr), NotNull<std::unique_ptr<Data>>>::value ) );
  ASSERT_TRUE(ptr);
  EXPECT_EQ( ptr->s_, "stuff" );
}


TEST_F(ButNotNull, MakeSharedNN)
{
  auto ptr = makeSharedNN<Data>("stuff");
  EXPECT_TRUE( ( std::is_same<decltype(ptr), NotNull<std::shared_ptr<Data>>>::value ) );
  ASSERT_TRUE(ptr);
  EXPECT_EQ( ptr->s_, "stuff" );

  auto ptr2 = ptr;
  EXPECT_EQ( ptr2.get(), ptr.get() );
}


struct NonCopyable final
{
  NonCopyable() = default;
  NonCopyable(NonCopyable const&) = delete;
  NonCopyable& operator=(NonCopyable const&) = delete;

  int answer() { return 42; }
};

TEST_F(ButNotNull, AsteriskOperatorReturnsReferenceNotValue)
{
  auto ptr = makeSharedNN<NonCopyable>();
  EXPECT_TRUE( ( std::is_same<NonCopyable&, decltype(*ptr)>::value ) );
  EXPECT_EQ( 42, (*ptr).answer() );     // this line should just compile
}


TEST_F(ButNotNull, TemplateUsingHelpersArePresent)
{
  // just a smoke test...
  {
    auto answer = 42;
    But::NotNullRaw<int> ptr = makeNN<int*>(&answer);
    EXPECT_EQ(42, *ptr);
  }
  {
    But::NotNullUnique<int> ptr = makeUniqueNN<int>(42);
    EXPECT_EQ(42, *ptr);
  }
  {
    But::NotNullShared<int> ptr = makeSharedNN<int>(42);
    EXPECT_EQ(42, *ptr);
  }
}


template<typename Unused>
void testCallWithTemplate()
{
  std::string str;
  But::NotNullRaw<std::string> ptr{&str};
  (void)ptr->size();    // this line caused error in GCC
}

TEST_F(ButNotNull, TestForGccBugWithArrowOperatorCalledFromTemplateFunction)
{
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81182
  testCallWithTemplate<int>();
}

}
