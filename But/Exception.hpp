#pragma once
#include <stdexcept>

namespace But
{

class Exception: public std::runtime_error
{
public:
  Exception(char const* file, unsigned line, char const* function, std::string msg);
};

}
