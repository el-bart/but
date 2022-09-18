#pragma once
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/toFieldInfo.hpp>
#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Destination/Common/rootElementTag.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace detail
{

template<typename ...Args>
auto args2FieldInfo(Args&& ...args)
{
  using ::But::Log::Backend::toFieldInfo;
  return Backend::FieldInfo{ Common::rootElementTag(), { toFieldInfo( std::forward<Args>(args) )... } };
}

}
}
}
}
