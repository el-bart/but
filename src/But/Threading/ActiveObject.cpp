#include <stdexcept>
#include <cassert>

#include "ActiveObject.hpp"

namespace But
{
namespace Threading
{

ActiveObject::ActiveObject():
  quit_{false},
  th_{&ActiveObject::processingLoop, this}
{ }


ActiveObject::~ActiveObject()
{
  try
  {
    quit_ = true;
    // unblock thread, if needed
    Queue::lock_type lock{q_};
    q_.push(Queue::value_type{});
  }
  catch(...)
  {
    assert(!"failed to stop thread - givin up...");
    abort();
  }
}


namespace
{
template<typename Q>
auto getCommand(Q& q)
{
  typename Q::lock_type lock{q};
  auto cmd = std::move( q.top(lock) );
  q.pop();
  return cmd;
}
}

void ActiveObject::processingLoop() noexcept
{
  while(not quit_)
  {
    try
    {
      auto cmd = getCommand(q_);
      if(cmd)
        cmd->run();
    }
    catch(...)
    {
      assert(!"unexpected exception from processing thread - this should never happen");
    }
  }
}

}
}

