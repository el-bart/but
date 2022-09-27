#include <gtest/gtest.h>
#include <But/Log/Backend/detail/backendSelector.hpp>
#include <But/Log/Backend/EntryProxy.hpp>
#include <But/Log/Backend/EntryArray.hpp>

using But::Log::Backend::EntryProxy;
using But::Log::Backend::EntryArray;
namespace D = But::Log::Backend::detail;

namespace
{

struct ButLogBackendDetailBackendSelector: public testing::Test
{
};


struct MyValue { };
int fieldValue(MyValue const&);

struct MyObject { };
void objectValue(EntryProxy&, MyObject const&);

struct MyArray { };
void arrayValue(EntryArray&, MyArray const&);


TEST_F(ButLogBackendDetailBackendSelector, CheckValue)
{
  EXPECT_TRUE(  D::HasFieldValue<MyValue >::value );
  EXPECT_FALSE( D::HasFieldValue<MyObject>::value );
  EXPECT_FALSE( D::HasFieldValue<MyArray >::value );
}


TEST_F(ButLogBackendDetailBackendSelector, CheckObject)
{
  EXPECT_FALSE( D::HasObjectValue<MyValue >::value );
  EXPECT_TRUE(  D::HasObjectValue<MyObject>::value );
  EXPECT_FALSE( D::HasObjectValue<MyArray >::value );
}


TEST_F(ButLogBackendDetailBackendSelector, CheckArray)
{
  EXPECT_FALSE( D::HasArrayValue<MyValue >::value );
  EXPECT_FALSE( D::HasArrayValue<MyObject>::value );
  EXPECT_TRUE(  D::HasArrayValue<MyArray >::value );
}


struct MyObjectV { };
void objectValue(EntryProxy, MyObjectV const&); // note: EntryProxy is passed by value

struct MyArrayV { };
void arrayValue(EntryArray, MyArrayV const&);   // note: EntryProxy is passed by value


TEST_F(ButLogBackendDetailBackendSelector, CheckObjectV)
{
  EXPECT_TRUE(  D::HasObjectValue<MyObjectV>::value );
  EXPECT_FALSE( D::HasObjectValue<MyArrayV >::value );
}


TEST_F(ButLogBackendDetailBackendSelector, CheckArrayV)
{
  EXPECT_FALSE( D::HasArrayValue<MyObjectV>::value );
  EXPECT_TRUE(  D::HasArrayValue<MyArrayV >::value );
}

}
