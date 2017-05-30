#pragma once

namespace But
{
namespace Threading
{
namespace detail
{

struct Command
{
  virtual ~Command() { }
  virtual void run() = 0;
};


template<typename F, typename Policy>
struct Task final: public Command
{
  explicit Task(F f): f_{ std::move(f) } { }

  virtual void run() noexcept
  {
    try
    {
      const auto tag = static_cast<value_type const*>(nullptr);
      runAndSetValue(tag);
    }
    catch(...)
    {
      promise_.set_exception( Policy::currentException() );
    }
  }

  void runAndSetValue(void const*)
  {
    f_();
    promise_.set_value();
  }

  template<typename Ignore>
  void runAndSetValue(Ignore const*)
  {
    promise_.set_value( f_() );
  }

  F f_;
  typename Policy::template promise_type<decltype(f_())> promise_;

  using value_type = typename std::decay<decltype(f_())>::type;
};

}
}
}
