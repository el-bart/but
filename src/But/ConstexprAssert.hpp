#pragma once
#include <cassert>

namespace But
{

#ifdef NDEBUG
// TODO: is it possible to enable constexpr assert only in constexpr context, in non-debug mode?
#define BUT_CONSTEXPR_ASSERT(cond) do { } while(false)
#else
#define BUT_CONSTEXPR_ASSERT(cond) do { if(not (cond)) assert(!#cond); } while(false)
#endif

}
