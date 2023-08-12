#pragma once
#include <string_view>
#include <But/Log/Backend/EntryProxy.hpp>
#include <But/Log/Backend/detail/backendSelector.hpp>

namespace But::Log::Field::detail
{

template<typename T>
struct DynamicallyNamedObject
{
  std::string_view name_;
  T nested_;
};

// NOTE: fieldName() is missing, as this type is NOT supposed to be used in a "regular" way.
//       instead it's considered an exception in fields processing framework.

template<typename T>
inline void objectValue(Backend::EntryProxy& proxy, DynamicallyNamedObject<T> const& v)
{
  // TODO:          
  if constexpr ( Backend::detail::HasObjectValue<T>::value )
    proxy.object(v.name_).nest(v.nested_);
  else
    if constexpr ( Backend::detail::HasFieldValue<T>::value )
      proxy.object(v.name_).nest(v.nested_);
    else
      if constexpr ( Backend::detail::HasArrayValue<T>::value )
        proxy.object(v.name_).nest(v.nested_);
      else
        proxy.value(v.name_, v.nested_);
}

}
