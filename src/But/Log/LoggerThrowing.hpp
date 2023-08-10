#pragma once
#include <But/NotNull.hpp>
#include <But/Format/apply.hpp>
#include <But/Format/format.hpp>
#include <But/Format/toString.hpp>
#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Localization/None.hpp>
#include <But/Log/Backend/detail/EntryRoot.hpp>
#include <But/Log/detail/allNamesUnique.hpp>

namespace But
{
namespace Log
{

/** @brief exception-throwing version of Logger class. use when logger errors are to be reported, too.
 *         aside from exceptions handling, this class shares all the other properties with Logger.
 *         see it's description for details.
 *
 * @warning throwing exceptions from logger is NOT a typical behaviour! unless you are absolutely sure what you are doing
 *          and this behavior is truely required, DON'T DO THIS! use Logger, that does not throw, instead.
 *          you have been warned.
 */
template<typename Translator = Localization::None>
class LoggerThrowing final
{
public:
  using Destination = But::NotNullShared<Destination::Sink>;

  explicit LoggerThrowing(Destination dst):
    LoggerThrowing{ std::move(dst), Translator{} }
  { }
  LoggerThrowing(Destination dst, Translator tr):
    LoggerThrowing{ std::move(dst), std::move(tr), Backend::detail::EntryRoot{} }
  { }

  template<typename ...Args>
  void log(std::string_view const message, Args&& ...args) const
  {
    auto root = entryRoot_.independentCopy();
    logImpl(root, message, std::forward<Args>(args)... );
  }

  template<size_t ArgumentsCount, size_t MaxSegments, typename ...Args>
  void log(Format::ParsedCompiletime<ArgumentsCount, MaxSegments> const& format, Args&& ...args) const
  {
    auto root = entryRoot_.independentCopy();
    auto proxy = root.proxy();
    addFormatNode( proxy, format, args... );
    logImpl(root, format.inputFormat(), std::forward<Args>(args)... );
  }

  template<typename ...Args>
  [[nodiscard]] auto withFields(Args&& ...args) const
  {
    auto root = entryRoot_.independentCopy();
    auto proxy = root.proxy();
    addFields( proxy, std::forward<Args>(args)... );
    return LoggerThrowing{ dst_, translator_, std::move(root) };
  }

  void reload() { dst_->reload(); }
  void flush() { dst_->flush(); }

private:
  LoggerThrowing(Destination dst, Translator tr, Backend::detail::EntryRoot entryRoot):
    dst_{ std::move(dst) },
    translator_{ std::move(tr) },
    entryRoot_{ std::move(entryRoot) }
  { }

  template<size_t ArgumentsCount, size_t MaxSegments, typename ...Args>
  void addFormatNode(Backend::EntryProxy& proxy, Format::ParsedCompiletime<ArgumentsCount, MaxSegments> const& format, Args const& ...args) const
  {
    auto fmt = proxy.object("But::Format");
    fmt.value("format", format.inputFormat());
    {
      auto array = fmt.array("args");
      addFields(array, args...);
    }
    {
      const auto translated = translator_->translate(format);
      auto formatted = Field::FormattedString{ Format::apply( translated, makeString(args)... ) };
      fmt.nest( std::move(formatted) );
    }
  }

  template<typename ...Args>
  void addFields(Backend::EntryProxy& proxy, Args&& ...args) const
  {
    static_assert( detail::allNamesUnique( fieldName( static_cast<Args const*>(nullptr) )... ),
                   "all parameters must be of a different type. if you are here - consider tagging" );
    ( proxy.nest( std::forward<Args>(args) ) , ... );
  }

  template<typename ...Args>
  void addFields(Backend::EntryArray& array, Args&& ...args) const
  {
    ( array.nest( std::forward<Args>(args) ) , ... );
  }

  template<typename ...Args>
  void logImpl(Backend::detail::EntryRoot& root, std::string_view const message, Args&& ...args) const
  {
    {
      auto proxy = root.proxy();
      proxy.value("message", message);
      addFields( proxy, std::forward<Args>(args)... );
    }
    dst_->log( root.json() );
  }

  template<typename T>
  std::string makeString(T const& t) const
  {
    if constexpr ( Backend::detail::HasToString<T>::value )
      return toString(t);
    else
    {
      if constexpr ( Backend::detail::HasFieldValue<T>::value )
        return Format::toString( fieldValue(t) );
      else
      {
        Backend::detail::EntryRoot er;
        er.proxy().nest(t);
        return er.json();
      }
    }
  }

  mutable Destination dst_;
  Translator translator_{};
  Backend::detail::EntryRoot entryRoot_;
};

}
}
