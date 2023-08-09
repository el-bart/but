#pragma once
#include <But/NotNull.hpp>
#include <But/Format/apply.hpp>
#include <But/Format/format.hpp>
#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Localization/None.hpp>
#include <But/Log/Backend/detail/EntryRoot.hpp>

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
template<typename Translator = Localization::None>
class ProxyThrowing final
{
public:
  using Destination = But::NotNullShared<Destination::Sink>;

  explicit ProxyThrowing(Destination dst):
    ProxyThrowing{ std::move(dst), Translator{} }
  { }
  ProxyThrowing(Destination dst, Translator tr):
    ProxyThrowing{ std::move(dst), std::move(tr), Backend::detail::EntryRoot{} }
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
    const auto translated = translator_->translate(format);
    auto formatted = Field::FormattedString{ Format::apply( translated, makeString(args)... ) };

    auto root = entryRoot_.independentCopy();
    {
      auto proxy = root.proxy();
      auto fmt = proxy.object("But::Format");
      fmt.value( "format", format.inputFormat() );
      auto array = fmt.array("args");
      addFields(array, args...);
    }
    logImpl(root, fieldValue(formatted), std::forward<Args>(args)... );
  }

  template<typename ...Args>
  [[nodiscard]] auto withFields(Args&& ...args) const
  {
    auto root = entryRoot_.independentCopy();
    addFields( root.proxy(), std::forward<Args>(args)... );
    return ProxyThrowing{ dst_, translator_, std::move(root) };
    /*
    auto tmp = convertSimplifiedArgs( std::forward<Args>(args)... );
    return ProxyThrowing{ dst_, translator_, makeSharedNN<Data>( std::move(tmp) ) };
    */
  }

  void reload() { dst_->reload(); }
  void flush() { dst_->flush(); }

private:
  //using Data = std::vector<Backend::FieldInfo>;

  ProxyThrowing(Destination dst, Translator tr, Backend::detail::EntryRoot entryRoot):
    dst_{ std::move(dst) },
    translator_{ std::move(tr) },
    entryRoot_{ std::move(entryRoot) }
  { }

  template<typename ...Args>
  void addFields(Backend::EntryProxy& proxy, Args&& ...args) const
  {
    // TODO: handle repeated field types...
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
    if constexpr ( Backend::detail::HasFieldValue<T>::value )
      return std::to_string( fieldValue(t) );
    else
    {
      Backend::detail::EntryRoot er;
      er.proxy().nest(t);
      return er.json();
    }
  }

  /*
  template<typename ...Args>
  [[nodiscard]] auto convertSimplifiedArgs(Args&& ...args) const
  {
    auto tmp = *entryRoot_;
    tmp.reserve( tmp.size() + sizeof...(args) );
    convertArgs( tmp, detail::simplifyRepresentation( std::forward<Args>(args) )... );
    return tmp;
  }

  void convertArgs(Data&) const { }

  template<typename Head, typename ...Tail>
  void convertArgs(Data& out, Head&& head, Tail&& ...tail) const
  {
    using Backend::toFieldInfo;
    out.push_back( toFieldInfo( std::forward<Head>(head) ) );
    convertArgs( out, std::forward<Tail>(tail)... );
  }

  template<size_t N, size_t M, typename ...Tail>
  void convertArgs(Data& out, Format::ParsedCompiletime<N,M>&& parsed, Tail&& ...tail) const
  {
    const auto translated = translator_->translate( std::move(parsed) );
    auto formatted = Field::FormattedString{ Format::apply(translated, tail...) };
    convertArgs( out, std::move(formatted), std::forward<Tail>(tail)... );
  }
  */

  mutable Destination dst_;
  Translator translator_{};
  Backend::detail::EntryRoot entryRoot_;
};

}
}
