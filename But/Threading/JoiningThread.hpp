#pragma once

#include <utility>

namespace But
{
namespace Threading
{

/** @brief automatic joiner of the underlying thread type (T).
 *  @note use with care, as join() withou proper interrupt can hang forever.
 */
template<typename T>
class JoiningThread final
{
public:
  using id = typename T::id;

  template<typename ...Args>
  explicit JoiningThread(Args&&... args):
    th_{ std::forward<Args>(args)... }
  { }

  JoiningThread(JoiningThread&&)  =default;
  JoiningThread& operator=(JoiningThread&&)  =default;

  ~JoiningThread()
  {
    if(joinable())
      join();
  }

  bool joinable() const { return th_.joinable(); }
  void join() { th_.join(); }
  void detach() { th_.detach(); }
  void swap(JoiningThread& other) { th_.swap(other.th_); }
  id get_id() const { th_.get_id(); }

private:
  T th_;
};

template<typename T>
void swap(JoiningThread<T>& lhs, JoiningThread<T>& rhs) { lhs.swap(rhs); }

}
}
