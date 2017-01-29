#include <initializer_list>
#include "gtest/gtest.h"
#include "FreeOperators.hpp"

namespace
{

struct ButMplFreeOperators: public testing::Test
{ };


struct MyData
{
  int value_;
};

BUT_MPL_FREE_OPERATORS_COMPARE(MyData, .value_)


TEST_F(ButMplFreeOperators, OperatorsForRegularClass)
{
  const MyData d1{13};
  const MyData d2{42};

  EXPECT_TRUE( d1 <  d2 );

  EXPECT_TRUE( d1 <= d2 );
  EXPECT_TRUE( d1 <= d1 );

  EXPECT_TRUE( d1 >= d1 );
  EXPECT_TRUE( d2 >= d1 );

  EXPECT_TRUE( d1 == d1 );
  EXPECT_TRUE( d1 != d2 );

  EXPECT_FALSE( d1 >  d2 );

  EXPECT_FALSE( d1 >= d2 );

  EXPECT_FALSE( d2 <= d1 );

  EXPECT_FALSE( d1 == d2 );
  EXPECT_FALSE( d1 != d1 );
}


template<typename T>
struct MyTemplateData
{
  T value_;
};

BUT_MPL_FREE_OPERATORS_TEMPLATE_COMPARE(MyTemplateData<T>, .value_, typename T)


TEST_F(ButMplFreeOperators, OperatorsForTemplateClass)
{
  const MyTemplateData<int> d1{13};
  const MyTemplateData<int> d2{42};

  EXPECT_TRUE( d1 <  d2 );

  EXPECT_TRUE( d1 <= d2 );
  EXPECT_TRUE( d1 <= d1 );

  EXPECT_TRUE( d1 >= d1 );
  EXPECT_TRUE( d2 >= d1 );

  EXPECT_TRUE( d1 == d1 );
  EXPECT_TRUE( d1 != d2 );

  EXPECT_FALSE( d1 >  d2 );

  EXPECT_FALSE( d1 >= d2 );

  EXPECT_FALSE( d2 <= d1 );

  EXPECT_FALSE( d1 == d2 );
  EXPECT_FALSE( d1 != d1 );
}


template<typename T, typename U>
struct My2TemplateData
{
  T value_;
};

#define CLASS My2TemplateData<T,U>
BUT_MPL_FREE_OPERATORS_TEMPLATE_COMPARE(CLASS, .value_, typename T, typename U)
#undef CLASS


TEST_F(ButMplFreeOperators, OperatorsForMultiTemplateClass)
{
  const My2TemplateData<int,double> d1{13};
  const My2TemplateData<int,double> d2{42};

  EXPECT_TRUE( d1 <  d2 );
}


struct MyCollection
{
  explicit MyCollection(std::initializer_list<std::string> lst): c_{lst} { }

  auto begin() const { return c_.begin(); }
  auto end() const { return c_.end(); }

  std::vector<std::string> c_;
};

BUT_MPL_FREE_OPERATORS_COLLECTION_COMPARE(MyCollection)


TEST_F(ButMplFreeOperators, OperatorsForCollection)
{
  const MyCollection mc1{ "ala",        "ma", "kota" };
  const MyCollection mc2{ "ala", "nie", "ma", "kota" };
  const MyCollection mc3{ "ala", "nie", "ma" };

  EXPECT_TRUE( mc3 < mc2 );

  EXPECT_TRUE( mc1 <= mc1 );
  EXPECT_TRUE( mc3 <= mc2 );

  EXPECT_TRUE( mc1 >= mc1 );
  EXPECT_TRUE( mc2 >= mc3 );

  EXPECT_TRUE( mc1 == mc1 );
  EXPECT_TRUE( mc1 != mc2 );

  EXPECT_FALSE( mc3 >  mc2 );

  EXPECT_FALSE( mc3 >= mc2 );

  EXPECT_FALSE( mc2 <= mc3 );

  EXPECT_FALSE( mc1 == mc2 );
  EXPECT_FALSE( mc1 != mc1 );
}


template<typename T>
struct MyTemplateCollection
{
  explicit MyTemplateCollection(std::initializer_list<T> lst): c_{lst} { }

  auto begin() const { return c_.begin(); }
  auto end() const { return c_.end(); }

  std::vector<T> c_;
};

BUT_MPL_FREE_OPERATORS_COLLECTION_TEMPLATE_COMPARE(MyTemplateCollection<T>, typename T)


TEST_F(ButMplFreeOperators, OperatorsForCollectionTemplate)
{
  using Collection = MyTemplateCollection<std::string>;
  const Collection c1{ "ala",        "ma", "kota" };
  const Collection c2{ "ala", "nie", "ma", "kota" };
  const Collection c3{ "ala", "nie", "ma" };

  EXPECT_TRUE( c3 < c2 );

  EXPECT_TRUE( c1 <= c1 );
  EXPECT_TRUE( c3 <= c2 );

  EXPECT_TRUE( c1 >= c1 );
  EXPECT_TRUE( c2 >= c3 );

  EXPECT_TRUE( c1 == c1 );
  EXPECT_TRUE( c1 != c2 );

  EXPECT_FALSE( c3 >  c2 );

  EXPECT_FALSE( c3 >= c2 );

  EXPECT_FALSE( c2 <= c3 );

  EXPECT_FALSE( c1 == c2 );
  EXPECT_FALSE( c1 != c1 );
}


template<typename T, typename U>
struct MyMultiTemplateCollection
{
  explicit MyMultiTemplateCollection(std::initializer_list<T> lst): c_{lst} { }

  auto begin() const { return c_.begin(); }
  auto end() const { return c_.end(); }

  std::vector<T> c_;
};

#define COLLECTION MyMultiTemplateCollection<T,U>
BUT_MPL_FREE_OPERATORS_COLLECTION_TEMPLATE_COMPARE(COLLECTION, typename T, typename U)
#undef COLLECTION


TEST_F(ButMplFreeOperators, OperatorsForCollectionMultiTemplate)
{
  using Collection = MyMultiTemplateCollection<std::string,int>;
  const Collection c1{ "ala",        "ma", "kota" };
  const Collection c2{ "ala", "nie", "ma", "kota" };

  EXPECT_TRUE( c1 < c2 );
}

}
