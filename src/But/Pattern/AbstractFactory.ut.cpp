#include <typeinfo>
#include "gmock/gmock.h"
#include "AbstractFactory.hpp"

using But::Pattern::AbstractFactory;

namespace
{

struct Base
{
  virtual ~Base() = default;
  virtual int foo() = 0;
};

struct ImplOne: public Base
{
  virtual int foo() override { return 1; }
};

auto buildOne() { return std::make_unique<ImplOne>(); }

struct ImplTwo: public Base
{
  virtual int foo() override { return 2; }
};

auto buildTwo() { return std::make_unique<ImplTwo>(); }

using Factory = AbstractFactory<Base>;

struct ButPatternAbstractFactory: public testing::Test
{
  Factory af_;
};


TEST_F(ButPatternAbstractFactory, AddingDuplicateEntry)
{
  const Factory::Builder name{"one"};
  af_.add(name, buildOne);
  EXPECT_THROW( (af_.add(name, buildOne)), Factory::DuplicatedEntry );
}


TEST_F(ButPatternAbstractFactory, AddingInvalidFunctor)
{
  EXPECT_THROW( (af_.add( Factory::Builder{"invalid"}, Factory::BuildFunctor{} )), Factory::InvalidBuilder );
}


TEST_F(ButPatternAbstractFactory, RemovingExistingEntry)
{
  const Factory::Builder name{"one"};
  af_.add(name, buildOne);
  EXPECT_TRUE( af_.remove(name) );
  EXPECT_THROW( af_.build(name), Factory::NoSuchBuilder );
}


TEST_F(ButPatternAbstractFactory, RemovingNonExistingEntry)
{
  EXPECT_FALSE( af_.remove(Factory::Builder{"nonexisting"}) );
}


TEST_F(ButPatternAbstractFactory, ReAddingAfterRemoval)
{
  const Factory::Builder name{"respawn"};
  af_.add(name, buildOne);
  EXPECT_TRUE( af_.remove(name) );
  af_.add(name, buildTwo);
  auto p = af_.build(name);
  ASSERT_TRUE( p.get() );
  EXPECT_EQ( 2, p->foo() );
}


TEST_F(ButPatternAbstractFactory, BuildingElements)
{
  af_.add(Factory::Builder{"one"}, buildOne);
  af_.add(Factory::Builder{"two"}, buildTwo);
  {
    auto e = af_.build(Factory::Builder{"one"});
    ASSERT_TRUE( e.get() );
    EXPECT_EQ( 1, e->foo() );
  }
  {
    auto e = af_.build(Factory::Builder{"two"});
    ASSERT_TRUE( e.get() );
    EXPECT_EQ( 2, e->foo() );
  }
}


struct ImplArgs: public Base
{
  ImplArgs(int i, std::string const& s):
    i_(i),
    s_(s)
  { }
  int         i_;
  std::string s_;
  virtual int foo() override { return 3; }
};

auto buildArgs(int i, std::string const& s) { return std::make_unique<ImplArgs>(i,s); }

using ArgsFactory = AbstractFactory<Base, int, std::string const&>;

TEST_F(ButPatternAbstractFactory, BuildingElementsWithArguments)
{
  ArgsFactory aaf;
  aaf.add(ArgsFactory::Builder{"sth"}, buildArgs);
  auto e = aaf.build(ArgsFactory::Builder{"sth"}, 42, "doom");
  ASSERT_TRUE( e.get() );
  auto const& derived = dynamic_cast<ImplArgs&>(*e);
  EXPECT_EQ( 42,     derived.i_ );
  EXPECT_EQ( "doom", derived.s_ );
}


TEST_F(ButPatternAbstractFactory, BuildingElementsWithFunctors)
{
  std::function<Factory::BasePtr()> f{buildOne};
  af_.add(Factory::Builder{"sth"}, f);
}


TEST_F(ButPatternAbstractFactory, BuildingElementsWithLambdas)
{
  af_.add( Factory::Builder{"sth"}, []{ return std::make_unique<ImplOne>(); } );
}


struct MyFunctor
{
  auto operator()() { return buildOne(); }
};

TEST_F(ButPatternAbstractFactory, BuildingElementsWithManualFunctors)
{
  af_.add(Factory::Builder{"sth"}, MyFunctor{});
}

}
