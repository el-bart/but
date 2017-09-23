#pragma once
#include "But/Format/format.hpp"
#include "Field/FormattedString.hpp"
#include "Localization/None.hpp"

namespace But
{
namespace Log
{

/** @brief exception-throwing version of LoggerProxy class. use when logger errors are to be reported, too.
 *         aside from exceptions handling, this class shares all the other properties with LoggerProxy.
 *         see it's description for details.
 *
 * @warning throwing exceptions from logger is NOT a typical behaviour! unless you are absolutely sure what you are doing
 *          and this behavior is truely required, DON'T DO THIS! use LoggerProxy, that does not throw, instead.
 *          you have been warned.
 */
template<typename Destination, typename Translator = Localization::None>
class LoggerProxyThrowing final
{
public:
  LoggerProxyThrowing() = default;
  explicit LoggerProxyThrowing(Destination dst): dst_{ std::move(dst) } { }
  explicit LoggerProxyThrowing(Translator tr): translator_{ std::move(tr) } { }
  LoggerProxyThrowing(Destination dst, Translator tr): dst_{ std::move(dst) }, translator_{ std::move(tr) } { }

  template<typename ...Args>
  void log(Args&& ...args) const { dst_->log( std::forward<Args>(args)... ); }

  template<unsigned N, unsigned M, typename ...Args>
  void log(Format::Parsed<N,M>&& parsed, Args&& ...args) const
  {
    const auto translated = translate( std::move(parsed) );
    const auto formatted = Field::FormattedString{ translated.format(args...) };
    dst_->log( formatted, std::forward<Args>(args)... );
  }

  void reload() { dst_->reload(); }
  void flush() { dst_->flush(); }

private:
  template<unsigned N, unsigned M>
  auto translate(Format::Parsed<N,M>&& parsed) const
  {
    try
    {
      auto copy = parsed;
      // TODO: log this as a warning?
      return translator_->translate( std::move(copy) );
    }
    catch(...)
    {
      return parsed;
    }
  }

  mutable Destination dst_{};
  const Translator translator_{};
};

}
}
