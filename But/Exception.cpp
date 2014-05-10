#include <array>
#include <limits>
#include <utility>
#include <cstring>
#include <cstdio>
#include <cassert>

#include "Exception.hpp"

namespace But
{

namespace
{
unsigned add(std::string &msg, const unsigned pos, char const* str)
{
  assert(str);
  assert(pos<msg.size());
  const auto sizePre = msg.size();
  msg.insert(pos, str);
  const auto sizePost = msg.size();
  return sizePost - sizePre;
}

template<unsigned NumLen>
unsigned addLine(std::string& msg, unsigned pos, unsigned line)
{
  std::array<char, NumLen+1> buf;
  auto len = snprintf( buf.data(), buf.size(), "%u", line);
  if(len<0)
  {
    static_assert(buf.size()>=2, "buffer not large enough to keep '\?\?' string inside");
    buf[0] = buf[1] = '?';
    buf[2] = 0;
    len = 2;
  }
  assert( static_cast<unsigned>(len) < buf.size() );
  assert( strlen(buf.data()) < buf.size() );
  return add(msg, pos, buf.data());
}

std::string constructErrorMessage(char const* file, unsigned line, char const* function, char const* defaultMsg, std::string msg)
{
  assert(file);
  assert(function);
  const     auto fileLen   = strlen(file);
  const     auto funcLen   = strlen(function);
  const     auto defMsgLen = strlen(defaultMsg);
  constexpr auto numLen    = std::numeric_limits<decltype(line)>::digits10 + 1;
  // "<file>:<line> <defMsg>: <msg> (in <func>)" + '\0'
  const auto totalLen = fileLen + 1 + defMsgLen + 2 + numLen + 1 + msg.size() + 5 + funcLen + 1 + 1;
  msg.reserve(totalLen);

  unsigned pos = 0;
  pos += add(msg, pos, file);
  pos += add(msg, pos, ":");
  pos += addLine<numLen>(msg, pos, line);
  pos += add(msg, pos, " ");
  pos += add(msg, pos, defaultMsg);
  pos += add(msg, pos, ": ");
  assert( pos < totalLen );
  msg.append(" (in ");
  msg.append(function);
  msg.append(")");

  assert( msg.size() <= totalLen && "something's wrong with max length computation" );
  return std::move(msg);
}
}


Exception::Exception(char const* file, unsigned line, char const* function, char const* defaultMsg, std::string msg):
  std::runtime_error{ constructErrorMessage(file, line, function, defaultMsg, std::move(msg)) }
{ }

}
