#pragma once

namespace But
{
namespace Log
{

/** @brief proxy class, to be used as an entrypoint for logging.
 *         Destination type can be anything that derives from Destination::Foregin class,
 *         or a user-provided type, that will handle all the arguments on its own.
 *
 * @example basic, zero-overhead, on-console output logger can be achieved like this:
 * <code>
 * using Log = LoggerProxy<Destination::Console>;
 * Log log;
 * log.log( Timestamp{}, " hello - current UTC date is: ", UtcDate{} );
 * </code>
 *
 * @example if a dynamic output is needed, use Destination::Foregin as a base class,
 *          for implementing any output for you want. as a parameter you will receive a
 *          collection of type-value pairs, reprenseting each argument.
 *
 * @note this class offers an option to either use your Destination directly (eg. Destination::Console),
 *       so that you can configure logger with a sinle typedef, or provide a runtime-dependent
 *       argument (i.e. Destination::Foregin derived class) and allow to change destination, depending
 *       on eg. config file entries.
 *
 * @warning it is "destination"'s implemented responsability to handile output in a thread-safe manner!
 */
template<typename Destination>
class LoggerProxy final
{
public:
  explicit LoggerProxy(Destination dst): dst_{ std::move(dst) } { }

  /** @brief creates a single log entry, out of a given parameters and strings.
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

  /** @brief triggers destination-specific reload actions (re-open output file, reconnect
   *         network socket, etc...). typicaly used for implementing easy log rotation.
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

private:
  mutable Destination dst_;
};

}
}
