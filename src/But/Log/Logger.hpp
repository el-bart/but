#pragma once
#include <But/Log/LoggerThrowing.hpp>

namespace But::Log
{

/** @brief logger class, to be used as an entry point for logging.
 *         Destination type can be anything that derives from Destination::Sink class,
 *         or a user-provided type, that will handle all the arguments on its own.
 *
 * @warning Logger is NOT thread-safe! "sinks" however are. what it means is one can re-use
 *          shared sinks between threads, but each thread shall have own copy of Logger object to use.
 *
 * @example basic, on-console output logger can be achieved like this:
 * <code>
 * using Log = But::Log::Logger<>
 * Log log{ std::make_shared<Destination::Console>() };
 * log.log( "hello, world!", Severity::Info, Timestamp{}, UtcDate{} );
 * </code>
 *
 * @example for testing ignore-all logger can be created like this:
 * <code>
 * using Log = But::Log::Logger<>
 * Log log{ std::make_shared<Destination::Null>() };
 * log.log( "hello, world", Severity::Info, Timestamp{}, UtcDate{} );
 * </code>
 *
 * @example multi-thread safe logging to file, with all attributes preserved, one JSON per line:
 * <code>
 * using Thread = But::JoiningThread<std::thread>;
 * using Log = But::Log::Logger<But::NotNullShared<Destination::Sink>;
 * Log log{ But::makeSharedNN<Destination::File>("/tmp/my_program.log") };
 * // note: each thread gets its own COPY of Logger object!
 * auto action = [log]() { log.log( "hello - UTC date!", Severity::Info, Timestamp{}, UtcDate{} ); };
 * Thread th1{action};
 * Thread th2{action};
 * </code>
 *
 * @warning it is sink's implementer responsibility to handle input/output in a thread-safe manner!
 *
 * @note all Destinations must either be smark pointers to base class Destination::Sink
 *
 * @note class allows parametrization with "translators", for having localization support (i.e. logs in different
 *       languages). by default no translation is being made.
 *
 * @note "translators" are only applicable for *formatted* logs!
 */
template<typename Translator = Localization::None>
class Logger final
{
public:
  using Backend = LoggerThrowing<Translator>;
  using Destination = typename Backend::Destination;

  explicit Logger(Destination dst): lpt_{ std::move(dst) } { }
  Logger(Destination dst, Translator tr): lpt_{ std::move(dst), std::move(tr) } { }

  /** @brief creates a single log entry, out of given parameters.
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

  /** @brief returns a logger object, that will always add a given set of fields (i.e. exact values!) into each log.
   *  @warning this creates new object, that allocates memory. it may throw!
   */
  template<typename ...Args>
  auto withFields(Args&& ...args) const
  {
    return Logger<Translator>{ lpt_.withFields( std::forward<Args>(args)... ) };
  }


private:
  explicit Logger(Backend backend): lpt_{ std::move(backend) } { }
  Backend lpt_;
};

}
