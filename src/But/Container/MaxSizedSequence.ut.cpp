#include "gtest/gtest.h"
#include "MaxSizedSequence.hpp"

using But::Container::MaxSizedSquence;

namespace
{

struct ButContainerMaxSizedSequence: public testing::Test
{
  using Sequence = MaxSizedSequence<std::string, 3>;

  Sequence        s_;
  Sequence const& cs_;
};


TEST_F(ButContainerMaxSizedSequence, NewlyCreatedCollectionIsEmpty)
{
}

}
