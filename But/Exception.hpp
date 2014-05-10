#pragma once
#include <stdexcept>

namespace But
{

class Exception: public std::runtime_error
{
protected:
  Exception(char const* file, unsigned line, char const* function, std::string msg);
};

}
