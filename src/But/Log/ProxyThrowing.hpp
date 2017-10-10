#pragma once
#include "But/Format/apply.hpp"
#include "But/Format/format.hpp"
#include "Field/FormattedString.hpp"
#include "Localization/None.hpp"

namespace But
{
namespace Log
{

/** @brief exception-throwing version of Proxy class. use when logger errors are to be reported, too.
 *         aside from exceptions handling, this class shares all the other properties with Proxy.
 *         see it's description for details.
 *
 * @warning throwing exceptions from logger is NOT a typical behaviour! unless you are absolutely sure what you are doing
 *          and this behavior is truely required, DON'T DO THIS! use Proxy, that does not throw, instead.
 *          you have been warned.
 */
template<typename Destination, typename Translator = Localization::None>
class ProxyThrowing final
{
public:
  ProxyThrowing() = default;
  explicit ProxyThrowing(Destination dst): dst_{ std::move(dst) } { }
  explicit ProxyThrowing(Translator tr): translator_{ std::move(tr) } { }
  ProxyThrowing(Destination dst, Translator tr): dst_{ std::move(dst) }, translator_{ std::move(tr) } { }

  template<typename ...Args>
  void log(Args&& ...args) const { dst_->log( std::forward<Args>(args)... ); }

  template<size_t N, size_t M, typename ...Args>
  void log(Format::ParsedCompiletime<N,M>&& parsed, Args&& ...args) const
  {
    const auto translated = translator_->translate( std::move(parsed) );
    const auto formatted = Field::FormattedString{ Format::apply(translated, args...) };
    dst_->log( formatted, std::forward<Args>(args)... );
  }

  void reload() { dst_->reload(); }
  void flush() { dst_->flush(); }

private:
  mutable Destination dst_{};
  const Translator translator_{};
};

}
}
