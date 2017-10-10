#pragma once
#include "ProxyThrowing.hpp"

namespace But
{
namespace Log
{

/** @brief proxy class, to be used as an entry point for logging.
 *         Destination type can be anything that derives from Destination::Sink class,
 *         or a user-provided type, that will handle all the arguments on its own.
 *
 * @warning Proxy is NOT thread-safe! "sinks" however are. what it means is one can re-use
 *          shared sinks between threads, but each thread shall have own copy of Proxy object to use.
 *
 * @example basic, on-console output logger can be achieved like this:
 * <code>
 * using Log = But::Log::Proxy<Destination::Console>;
 * Log log;
 * log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} );
 * </code>
 *
 * @example the most trivial, near-to-zero overhead, on-console output logger can be achieved like this:
 * <code>
 * using Log = But::Log::Proxy<Destination::TextConsole>;
 * Log log;
 * log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} );
 * </code>
 *
 * @example multi-thread safe logging to file, with all attributes preserved, one JSON per line:
 * <code>
 * using Thread = But::JoiningThread<std::thread>;
 * using Log = But::Log::Proxy<But::NotNullShared<Destination::Sink>;
 * Log log{ But::makeSharedNN<Destination::JsonFile>("/tmp/my_program.log") };
 * // note: each thread gets its own COPY of Proxy object!
 * auto action = [log]() { log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} ); };
 * Thread th1{action};
 * Thread th2{action};
 * </code>
 *
 * @example if a dynamic output is needed, use Destination::Sink as a base class,
 *          for implementing any output for you want. as a parameter you will receive a
 *          collection of type-value pairs, representing each argument.
 *
 * @warning it is sink's implementer responsibility to handle input/output in a thread-safe manner!
 *
 * @note all Destinations must either be (smart) pointers or provide an arrow operator.
 *
 * @note class allows parametrization with "translators", for having localization support (i.e. logs in different
 *       languages). by default no translation is being made.
 *
 * @note "translators" are only applicable for *formatted* logs!
 */
template<typename Translator = Localization::None>
class Proxy final
{
  using Impl = ProxyThrowing<Translator>;

public:
  using Destination = typename Impl::Destination;

  explicit Proxy(Destination dst): lpt_{ std::move(dst) } { }
  Proxy(Destination dst, Translator tr): lpt_{ std::move(dst), std::move(tr) } { }

  /** @brief creates a single log entry, out of a given parameters.
   */
  template<typename ...Args>
  void log(Args&& ...args) const noexcept
  {
    try
    {
      lpt_.log( std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief creates a single log entry, of a formatted-string for, out of a given parameters. tags are preserved.
   */
  template<size_t N, size_t M, typename ...Args>
  void log(Format::ParsedCompiletime<N,M>&& parsed, Args&& ...args) const noexcept
  {
    try
    {
      lpt_.log( std::move(parsed), std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief triggers sink-specific reload actions (re-open output file, reconnect
   *         network socket, etc...). typically used for implementing easy log rotation.
   */
  void reload() noexcept
  {
    try
    {
      lpt_.reload();
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief forces all buffered data (if any) to be flushed to its destination. this call may block.
   */
  void flush() noexcept
  {
    try
    {
      lpt_.flush();
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

private:
  Impl lpt_;
};

}
}
