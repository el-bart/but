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
  Exception(char const* file, unsigned line, char const* function, char const* defaultMsg, std::string msg);
};

#define BUT_DEFINE_EXCEPTION(name, base, msgDef) struct name: public base { name(char const* file, unsigned line, char const* func, std::string msg): base(file, line, func, msgDef, std::move(msg)) { }   name(char const* file, unsigned line, char const* func, char const* derivedMsgDef, std::string msg): base(file, line, func, derivedMsgDef, std::move(msg)) { } };

#define BUT_THROW(type, msgExpr) do{ std::stringstream ss; ss << msgExpr; throw type{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, ss.str()}; }while(false);

}
