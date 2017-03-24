#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Foregin.hpp"

using testing::_;
using But::Log::Destination::Foregin;
using But::Log::Backend::Entry;
using But::Log::Backend::FieldInfo;

namespace But
{
namespace Log
{
namespace Destination
{

struct ForeginMock: public Foregin
{
  MOCK_METHOD1(logImpl, void(Entry));
  MOCK_METHOD0(reloadImpl, void());
  MOCK_METHOD0(flushImpl, void());
};

}
}
}
