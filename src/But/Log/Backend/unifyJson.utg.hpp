#pragma once
#include <gtest/gtest.h>
#include <But/Log/Backend/detail/unifyJson.ut.hpp>

namespace But::Log::Backend
{

#define EXPECT_EQ_JSON(a, b) \
        EXPECT_EQ( ::But::Log::Backend::detail::unifyJson(a), ::But::Log::Backend::detail::unifyJson(b) )

}
