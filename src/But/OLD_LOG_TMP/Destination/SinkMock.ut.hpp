#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

struct SinkMock: public Sink
{
  MOCK_METHOD1(logImpl, void(::But::Log::Backend::FieldInfo const&));
  MOCK_METHOD0(reloadImpl, void());
  MOCK_METHOD0(flushImpl, void());
};

}
}
}
