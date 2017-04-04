#pragma once
#include <sstream>
#include <utility>
#include <stdexcept>
#include <boost/current_function.hpp>

namespace But
{
namespace detail
{

#define BUT_DEFINE_EXCEPTION_IMPL(name, base, msgDef) \
  struct name: public base \
  { \
    name(char const* file, unsigned line, char const* func, std::string msg): \
      name{ defineBegining(file, line), func, std::move(msg) } \
    { } \
  protected: \
    explicit name(std::stringstream&& ss): base{ std::move(ss) } { } \
    static std::stringstream&& appendDefaultMessage(std::stringstream&& ss) \
    { \
      auto&& out = base::appendDefaultMessage( std::move(ss) ); \
      out << msgDef << ": "; \
      return std::move(out); \
    } \
  private: \
    name(std::stringstream&& ss, char const* func, std::string&& msg): \
      base{ defineEnding( appendMessage( appendDefaultMessage( std::move(ss) ), std::move(msg) ), func ) } \
    { } \
  }


#define BUT_THROW_IMPL(type, msgExpr) \
  do \
  { \
    std::stringstream ss; \
    ss << msgExpr; \
    throw type{ __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, ss.str() }; \
  } while(false)

}
}
