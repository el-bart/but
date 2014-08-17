#pragma once
#include "gmock/gmock.h"
#include "But/Pattern/Dispatcher.hpp"
#include "But/Mpl/NamedVariadicTemplate.hpp"

namespace But
{
namespace Pattern
{
namespace detail
{

template<typename M>
struct MockMethod
{
  MOCK_METHOD1_T(handle, void(M const&));

protected:
  ~MockMethod() = default;
};

template<typename ...Msgs>
struct MockAll
{ };

template<typename Head, typename ...Tail>
struct MockAll<Head, Tail...>: public MockMethod<Head>,
                               public MockAll<Tail...>
{ };

template<typename Disp, typename ...Msgs>
struct DispatcherMockImpl;

template<typename Disp, typename ...Msgs>
struct DispatcherMockImpl<Disp, Mpl::NamedVariadicTemplate<Msgs...>> final:
  public AutoDispatcher< DispatcherMockImpl<Disp, Mpl::NamedVariadicTemplate<Msgs...> >,
                         typename Disp::PolicyType,
                         Msgs... >,
  public MockAll<Msgs...>
{
  template<typename M>
  void handle(M const& m)
  {
    static_cast<MockMethod<M>&>(*this).handle(m);
  }
};

}
}
}
