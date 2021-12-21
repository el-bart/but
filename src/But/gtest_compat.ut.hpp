#pragma once

// gtest compatibility thing
#ifdef TYPED_TEST_SUITE_P
// new API
#define BUT_TYPED_TEST_SUITE_P              TYPED_TEST_SUITE_P
#define BUT_REGISTER_TYPED_TEST_SUITE_P     REGISTER_TYPED_TEST_SUITE_P
#define BUT_INSTANTIATE_TYPED_TEST_SUITE_P  INSTANTIATE_TYPED_TEST_SUITE_P
#else
// old API
#define BUT_TYPED_TEST_SUITE_P              TYPED_TEST_CASE_P
#define BUT_REGISTER_TYPED_TEST_SUITE_P     REGISTER_TYPED_TEST_CASE_P
#define BUT_INSTANTIATE_TYPED_TEST_SUITE_P  INSTANTIATE_TYPED_TEST_CASE_P
#endif