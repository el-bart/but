#pragma once
#include "But/Format/format.hpp"
#include "Field/FormattedString.hpp"

namespace But
{
namespace Log
{

/** @brief proxy class, to be used as an entry point for logging.
 *         Destination type can be anything that derives from Destination::Foreign class,
 *         or a user-provided type, that will handle all the arguments on its own.
 *
 * @warning LoggerProxy is NOT thread-safe! "destinations" however are. what it means is one can re-use
 *          shared destinations between threads, but each thread shall have own copy of LoggerProxy object to use.
 *
 * @example basic, on-console output logger can be achieved like this:
 * <code>
 * using Log = LoggerProxy<Destination::Console>;
 * Log log;
 * log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} );
 * </code>
 *
 * @example the most trivial, near-to-zero overhead, on-console output logger can be achieved like this:
 * <code>
 * using Log = LoggerProxy<Destination::NaiveConsole>;
 * Log log;
 * log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} );
 * </code>
 *
 * @example multi-thread safe logging to file, with all attributes preserved, one JSON per line:
 * <code>
 * using Thread = But::JoiningThread<std::thread>;
 * using Log = LoggerProxy<But::NotNullShared<Destination::Foreign>;
 * Log log{ But::makeSharedNN<Destination::JsonFile>("/tmp/my_program.log") };
 * // note: each thread gets its own COPY of LoggerProxy object!
 * auto action = [log]() { log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} ); };
 * Thread th1{action};
 * Thread th2{action};
 * </code>
 *
 * @example if a dynamic output is needed, use Destination::Foreign as a base class,
 *          for implementing any output for you want. as a parameter you will receive a
 *          collection of type-value pairs, representing each argument.
 *
 * @note this class offers an option to either use your Destination directly (eg. Destination::Console),
 *       so that you can configure logger with a single typedef, or provide a runtime-dependent
 *       argument (i.e. Destination::Foreign derived class) and allow to change destination, depending
 *       on eg. config file entries.
 *
 * @warning it is "destination"'s implementer responsibility to handle input/output in a thread-safe manner!
 *
 * @note all Destinations must either be (smart) pointers or provide an arrow operator.
 */
template<typename Destination>
class LoggerProxy final
{
public:
  explicit LoggerProxy(Destination dst): dst_{ std::move(dst) } { }

  /** @brief creates a single log entry, out of a given parameters.
   */
  template<typename ...Args>
  void log(Args&& ...args) const
  {
    try
    {
      dst_->log( std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief creates a single log entry, of a formatted-string for, out of a given parameters. tags are preserved.
   */
  template<unsigned N, unsigned M, typename ...Args>
  void log(Format::Parsed<N,M>&& parsed, Args&& ...args) const
  {
    try
    {
      const auto formatted = Field::FormattedString{ parsed.format(args...) };
      dst_->log( formatted, std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief triggers destination-specific reload actions (re-open output file, reconnect
   *         network socket, etc...). typically used for implementing easy log rotation.
   */
  void reload()
  {
    try
    {
      dst_->reload();
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

  /** @brief forces all buffered data (if any) to be flushed to its destination. this call may block.
   */
  void flush()
  {
    try
    {
      dst_->flush();
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

private:
  mutable Destination dst_;
};

}
}
