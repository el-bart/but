#pragma once

#ifndef CATCH2_VERSION_MAJOR
#error "CATCH2_VERSION_MAJOR is not defined"
#endif

#if CATCH2_VERSION_MAJOR == 3
#include <catch2/catch_all.hpp>
#else
#if CATCH2_VERSION_MAJOR == 2
#include <catch2/catch.hpp>
#else
#error "unsupported catch2 version"
#endif // v2
#endif // v3
