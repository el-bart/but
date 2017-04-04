#pragma once
#include <sstream>
#include <utility>
#include <stdexcept>
#include <boost/current_function.hpp>

namespace But
{

class Exception: public std::runtime_error
{
protected:
  explicit Exception(std::stringstream&& ss): std::runtime_error{ ss.str() } { }
  //Exception(char const* file, unsigned line, char const* function, char const* defaultMsg, std::string msg);

  static std::stringstream   defineBegining(char const* file, unsigned line);
  static std::stringstream&& defineEnding(std::stringstream&& ss, char const* function);
  static std::stringstream&& appendMessage(std::stringstream&& ss, std::string&& msg) { ss << std::move(msg); return std::move(ss); }
  static std::stringstream&& appendDefaultMessage(std::stringstream&& ss) { return std::move(ss); }
};


#define BUT_DEFINE_EXCEPTION(name, base, msgDef) \
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


#define BUT_THROW(type, msgExpr) \
  do \
  { \
    std::stringstream ss; \
    ss << msgExpr; \
    throw type{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, ss.str()}; \
  } while(false)

}
