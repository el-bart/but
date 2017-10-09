#pragma once
#include "But/Format/Parsed.hpp"
#include "argumentsCountWithChecks.hpp"

namespace But
{
namespace Format
{
namespace detail
{

#define BUT_FORMAT_DETAIL_FORMAT_COMPILETIME_IMPL(fmt, N) \
    ::But::Format::ParsedCompiletime< ::But::Format::detail::argumentsCountWithChecks<N>(fmt), N >(fmt)

#define BUT_FORMAT_DETAIL_FORMAT_RUNTIME_IMPL(fmt) \
    ::But::Format::ParsedRuntime{fmt}

}
}
}
