#pragma once

namespace But
{
namespace Log
{

enum class Priority
{
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

char const* toString(const Priority pri) noexcept;

}
}
