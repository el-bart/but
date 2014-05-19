#pragma once

#include <sstream>
#include <iostream>
#include <boost/current_function.hpp>

#include "But/Log/Priority.hpp"

namespace But
{
namespace Log
{
namespace detail
{

void logProxy(const Priority p, char const* file, unsigned line, char const* function, std::string msg) noexcept;

#define BUT_LOG_DETAIL_LOGIMPL_BODY(pri, msg) std::stringstream ss; ss << msg; ::But::Log::detail::logProxy( ::But::Log::Priority::pri, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, ss.str() )
#define BUT_LOG_DETAIL_LOGIMPL(pri, msg) try { BUT_LOG_DETAIL_LOGIMPL_BODY(pri,msg); } catch(...) { std::cerr << "But::Log: failed to log due to exception" << std::endl; }

}
}
}
