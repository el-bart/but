#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

struct ForeignMock: public Foreign
{
  MOCK_METHOD1(logImpl, void(::But::Log::Backend::Entry));
  MOCK_METHOD0(reloadImpl, void());
  MOCK_METHOD0(flushImpl, void());
};

}
}
}
