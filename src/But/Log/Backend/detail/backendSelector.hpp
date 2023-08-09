#pragma once
#include <type_traits>

namespace But::Log::Backend
{

struct EntryProxy;
struct EntryArray;

namespace detail
{

template<typename T>
struct HasFieldValue
{
  template<typename U, typename = decltype( fieldValue( std::declval<U>() ) )>
  static std::true_type test(int);

  template <typename U>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(0))::value;
};


template<typename T>
struct HasObjectValue
{
  template<typename U, typename = decltype( objectValue( std::declval<EntryProxy&>(), std::declval<U>() ) )>
  static std::true_type test(int);

  template <typename U>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(0))::value;
};


template<typename T>
struct HasArrayValue
{
  template<typename U, typename = decltype( arrayValue( std::declval<EntryArray&>(), std::declval<U>() ) )>
  static std::true_type test(int);

  template <typename U>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(0))::value;
};


template<typename T>
struct HasToString
{
  template<typename U, typename = decltype( toString( std::declval<U>() ) )>
  static std::true_type test(int);

  template <typename U>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(0))::value;
};

}
}
