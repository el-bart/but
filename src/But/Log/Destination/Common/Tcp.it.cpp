#include "gtest/gtest.h"
#include "Tcp.hpp"

using But::Log::Destination::Common::Tcp;

namespace
{

struct ButLogDestinationCommonTcp: public testing::Test
{
};


TEST_F(ButLogDestinationCommonTcp, NoConnectionByDefault)
{
  // TODO...
}


TEST_F(ButLogDestinationCommonTcp, ConnectingWhenSending)
{
  // TODO...
}


TEST_F(ButLogDestinationCommonTcp, ThrowingOnIoError)
{
  // TODO...
}


TEST_F(ButLogDestinationCommonTcp, ReconnectingOnConnectionLoss)
{
  // TODO...
}

}
