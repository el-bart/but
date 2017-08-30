#pragma once
#include "Optional.hpp"
#include "assert.hpp"

namespace But
{

template<typename F>
class Guard final
{
public:
  Guard(F&& f): f_{ std::forward<F>(f) } { }
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
auto makeGuard(F&& f) { return Guard<F>{ std::forward<F>(f) }; }

}
