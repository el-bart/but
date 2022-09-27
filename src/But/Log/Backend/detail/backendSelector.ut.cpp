#include <gtest/gtest.h>
#include <But/Log/Backend/detail/backendSelector.hpp>
#include <But/Log/Backend/EntryRoot.hpp>

using But::Log::Backend::EntryRoot;
using But::Log::Backend::EntryProxy;
namespace D = But::Log::Backend::detail;

namespace
{

struct ButLogBackendDetailBackendSelector: public testing::Test
{
  EntryRoot er_;
  EntryProxy ep_{ er_.proxy() };
};


struct MyValue { };
void fieldValue(EntryProxy&, MyValue const&);

struct MyObject { };
void objectValue(EntryProxy&, MyObject const&);

struct MyArray { };
void arrayValue(EntryProxy&, MyArray const&);


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

}
