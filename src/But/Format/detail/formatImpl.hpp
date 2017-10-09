#pragma once
#include "But/Format/Parsed.hpp"
#include "argumentsCountWithChecks.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<size_t ArgumentsCount>
struct CompiletimeCheckedFormatProxy final
{
  static auto make(char const* fmt) { return Parsed{fmt}; }
};

#define BUT_FORMAT_DETAIL_FORMAT_COMPILETIME_IMPL(fmt, N) \
    ::But::Format::detail::CompiletimeChckedFormatProxy< ::But::Format::detail::argumentsCountWithChecks<N>(fmt) >::make(fmt)

#define BUT_FORMAT_DETAIL_FORMAT_RUNTIME_IMPL(fmt) \
    ::But::Format::Parsed{fmt}

}
}
}
