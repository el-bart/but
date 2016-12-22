#pragma once

namespace But
{
namespace Pattern
{
namespace detail
{

template<typename FinalType, typename M>
void callForwarder(FinalType& ft, typename FinalType::BinaryFormat const& bin)
{
  ft.handle( FinalType::template deserialize<M>(bin) );
}

template<typename FinalType, typename Head, typename ...Tail>
struct Registrator
{
  template<typename Collection>
  static void call(Collection& c)
  {
    constexpr auto id = FinalType::template getId<Head>();
    c[id] = &callForwarder<FinalType, Head>;
    Registrator<FinalType, Tail...>::call(c);
  }
};

template<typename FinalType>
struct Registrator<FinalType, void>
{
  template<typename Collection>
  static void call(Collection&) { }
};

}
}
}
