// this header should not be included directly in client code!
// it's here as a workarund for a GCC bug.
#pragma once

namespace But::Log::Backend
{

template<typename T>
void EntryArray::nest(T const& t)
{
  if constexpr ( detail::HasObjectValue<T>::value )
  {
    auto p = object();
    objectValue(p, t);
  }
  else
    if constexpr ( detail::HasFieldValue<T>::value )
      value( fieldValue(t) );
    else
      if constexpr ( detail::HasArrayValue<T>::value )
      {
        auto a = array();
        arrayValue(a, t);
      }
      else
        value(t);
}

}
