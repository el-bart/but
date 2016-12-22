#include "api.hpp"

int main()
{
  BUT_LOG(debug,   "internall stuff");
  BUT_LOG(info,    "hello there");
  BUT_LOG(warning, "oops");
  BUT_LOG(error,   "wtf?");
  BUT_LOG(fatal,   "omg - we're screwed");
}
