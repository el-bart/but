#include <But/ExplicitInit.hpp>
#include <But/detail/catch2.hpp>
#include <string>

using But::ExplicitInit;

using EiStr = ExplicitInit<std::string>;
using EiInt = ExplicitInit<int>;
using EiPtr = ExplicitInit<std::unique_ptr<char[]>>;

namespace
{

TEST_CASE("ExplicitInit: operator access (const)")
{
  EiStr const s{"test"};
  CHECK( s.get() == "test" );
  CHECK( s->data() == std::string{"test"} );
  CHECK( *s == "test" );
}

TEST_CASE("ExplicitInit: operator access (non-const)")
{
  EiStr s{"test"};

  SECTION("reading")
  {
    CHECK( s.get() == "test" );
    CHECK( s->data() == std::string{"test"} );
    CHECK( *s == "test" );
  }

  SECTION("writing via get()")
  {
    s.get().push_back('!');
    CHECK( s.get() == "test!" );
  }

  SECTION("writing via operator*()")
  {
    (*s).push_back('!');
    CHECK( s.get() == "test!" );
  }

  SECTION("writing via operator->()")
  {
    s->push_back('!');
    CHECK( s.get() == "test!" );
  }
}

#if 0
TEST_CASE("ExplicitInit: different init forms")
{
  EiStr const s1{"test"};
}

TEST_CASE("ExplicitInit: 1-arg c-tor")
{
  EiStr const s1{"test"};
}

TEST_CASE("ExplicitInit: multi-arg c-tor")
{
  EiStr const s1{"test"};
}

TEST_CASE("ExplicitInit: copying")
{
  EiStr const s1{"test"};
}

TEST_CASE("ExplicitInit: moving")
{
  EiStr const s1{"test"};
}
#endif

}
