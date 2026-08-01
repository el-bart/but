#pragma once
#include <utility>

namespace But
{

template<typename T>
class ExplicitInit final
{
public:
  using element_type = T;

  template<typename ...Args>
  explicit ExplicitInit(Args&& ...args):
    t_{ std::forward<Args>(args)... }
  { }

  ExplicitInit() = delete;
  ~ExplicitInit() = default;

  ExplicitInit(ExplicitInit &) = default;
  ExplicitInit& operator=(ExplicitInit &) = default;
  ExplicitInit(ExplicitInit const&) = default;
  ExplicitInit& operator=(ExplicitInit const&) = default;
  ExplicitInit(ExplicitInit &&) = default;
  ExplicitInit& operator=(ExplicitInit &&) = default;

  auto& get() const        { return t_; }
  auto* operator->() const { return &t_; }
  auto& operator*()  const { return get(); }

  auto& get()              { return t_; }
  auto* operator->()       { return &t_; }
  auto& operator*()        { return get(); }

private:
  T t_;
};

}
