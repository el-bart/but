#pragma once
#include <type_traits>
#include <But/Optional.hpp>
#include <But/assert.hpp>

namespace But
{

template<typename F>
class Guard final
{
public:
  static_assert( not std::is_reference<F>::value, "guard functors can only be kept by value" );

  Guard(F const& f): f_{f} { }
  Guard(F&& f): f_{ std::move(f) } { }
  ~Guard() noexcept
  {
    if(f_)
      (*f_)();
  }

  Guard(Guard&&) = default;
  Guard& operator=(Guard&&) = default;

  Guard(Guard const&) = delete;
  Guard& operator=(Guard const&) = delete;

  void cancel() { f_.reset(); }

private:
  Optional<F> f_;
};


template<typename F>
auto makeGuard(F&& f) { return Guard< typename std::remove_reference<F>::type >{ std::forward<F>(f) }; }

}
