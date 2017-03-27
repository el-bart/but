#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Foreign.hpp"

using testing::_;
using But::Log::Destination::Foreign;
using But::Log::Backend::Entry;
using But::Log::Backend::FieldInfo;

namespace But
{
namespace Log
{
namespace Destination
{

struct ForeignMock: public Foreign
{
  MOCK_METHOD1(logImpl, void(Entry));
  MOCK_METHOD0(reloadImpl, void());
  MOCK_METHOD0(flushImpl, void());
};

}
}
}
