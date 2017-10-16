#pragma once
#include <sstream>
#include <utility>
#include <stdexcept>
#include <But/detail/Exception.hpp>

namespace But
{

class Exception: public std::runtime_error
{
protected:
  explicit Exception(std::stringstream&& ss): std::runtime_error{ ss.str() } { }

  static std::stringstream   defineBegining(char const* file, unsigned line);
  static std::stringstream&& defineEnding(std::stringstream&& ss, char const* function);
  static std::stringstream&& appendMessage(std::stringstream&& ss, std::string&& msg) { ss << std::move(msg); return std::move(ss); }
  static std::stringstream&& appendDefaultMessage(std::stringstream&& ss) { return std::move(ss); }
};

#define BUT_DEFINE_EXCEPTION(name, base, msgDef) BUT_DEFINE_EXCEPTION_IMPL(name, base, msgDef)

#define BUT_THROW(type, msgExpr) BUT_THROW_IMPL(type, msgExpr)

}
