#pragma once

#include "But/Log/detail/logImpl.hpp"
#include "But/Log/Priority.hpp"

namespace But
{
namespace Log
{

#define BUT_LOG(pri, msg) do { BUT_LOG_DETAIL_LOGIMPL(pri, msg); } while(false)

}
}
