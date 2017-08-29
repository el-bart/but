#pragma once
#include <cassert>

namespace But
{

#ifdef NDEBUG
#define BUT_CONSTEXPR_ASSERT(cond) do { } while(false)
#else
#define BUT_CONSTEXPR_ASSERT(cond) do { if(not (cond)) assert(!#cond); } while(false)
#endif

}
