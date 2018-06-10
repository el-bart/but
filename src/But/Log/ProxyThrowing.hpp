#pragma once
#include <But/NotNull.hpp>
#include <But/Format/apply.hpp>
#include <But/Format/format.hpp>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/toFieldInfo.hpp>
#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Destination/Common/rootElementTag.hpp>
#include <But/Log/Localization/None.hpp>
#include <But/Log/detail/simplifyRepresentation.hpp>

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
    ProxyThrowing{ std::move(dst), std::move(tr), makeSharedNN<Data>() }
  { }

  template<typename ...Args>
  void log(Args&& ...args) const
  {
    auto tmp = convertSimplifiedArgs( std::forward<Args>(args)... );
    Backend::FieldInfo fi{ Log::Destination::Common::rootElementTag(), std::move(tmp) };
    dst_->log( std::move(fi) );
  }

  template<typename ...Args>
  auto withFields(Args&& ...args) const
  {
    auto tmp = convertSimplifiedArgs( std::forward<Args>(args)... );
    return ProxyThrowing{ dst_, translator_, makeSharedNN<Data>( std::move(tmp) ) };
  }

  void reload() { dst_->reload(); }
  void flush() { dst_->flush(); }

private:
  using Data = std::vector<Backend::FieldInfo>;

  ProxyThrowing(Destination dst, Translator tr, NotNullShared<Data> commonFields):
    dst_{ std::move(dst) },
    translator_{ std::move(tr) },
    commonFields_{ std::move(commonFields) }
  { }

  template<typename ...Args>
  auto convertSimplifiedArgs(Args&& ...args) const
  {
    auto tmp = *commonFields_;
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

  mutable Destination dst_;
  Translator translator_{};
  NotNullShared<Data> commonFields_;
};

}
}
