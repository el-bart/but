#pragma once
#include "gmock/gmock.h"
#include "detail/DispatcherMockImpl.hpp"

namespace But
{
namespace Pattern
{

/** @brief defines a mock type. mocking is done automatically.
 *  @param Type type name of a dispatcher to be mocked.
 *  @note if compilation time is an issue, extern template mechanism can be used to speed things up.
 */
#define BUT_PATTERN_DISPATCHER_MOCK(Type)  But::Pattern::detail::DispatcherMockImpl<Type, Type::Messages>

/** @brief set expectation of receiving a given message, with a given arguments.
 *  @param mock mock object to set expectation on.
 *  @param Msg  type name of an expected mesage.
 *  @note user needs to implemente comparison operator for a given mesage type to make this work.
 */
#define BUT_PATTERN_DISPATCHER_EXPECT_CALL(mock, Msg, ...)  EXPECT_CALL( static_cast<But::Pattern::detail::MockMethod<Msg>&>(mock),  handle( Msg{__VA_ARGS__} ) )

}
}
