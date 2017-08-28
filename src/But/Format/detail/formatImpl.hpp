#pragma once
#include "But/Format/Parsed.hpp"
#include "argumentsCountWithChecks.hpp"

namespace But
{
namespace Format
{
namespace detail
{

#define BUT_FORMAT_DETAIL_FORMAT_IMPL(fmt, N) \
    ::But::Format::Parsed< ::But::Format::detail::argumentsCountWithChecks<N>(fmt), N >{ fmt }

}
}
}
