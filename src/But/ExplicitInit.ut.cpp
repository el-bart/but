#include <But/ExplicitInit.hpp>
#include <But/detail/catch2.hpp>
#include <string>

using But::ExplicitInit;

using EiStr = ExplicitInit<std::string>;
using EiPtr = ExplicitInit<std::unique_ptr<char[]>>;

namespace
{

#if 0
TEST_CASE("ExplicitInit: default c-tor should not compile")
{
  EiStr s;
}
#endif

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


TEST_CASE("ExplicitInit: different init forms")
{
  SECTION("parentheses-init")
  {
    EiStr const s("test");
    CHECK(*s == "test");
  }

  SECTION("brace-init")
  {
    EiStr const s{"test"};
    CHECK(*s == "test");
  }

  SECTION("explicit assignment-init")
  {
    EiStr const s = std::string{"test"};
    CHECK(*s == "test");
  }

  SECTION("implicit assignment-init")
  {
    EiStr const s = "test";
    CHECK(*s == "test");
  }

  SECTION("initialized-list-init")
  {
    EiStr const s = {'t', 'e', 's', 't'};
    CHECK(*s == "test");
  }
}


TEST_CASE("ExplicitInit: multi-arg c-tor")
{
  using T = std::tuple<int, double>;
  ExplicitInit<T> t{42, 4.5};
  CHECK( *t == T{42, 4.5} );
}


TEST_CASE("ExplicitInit: copying")
{
  SECTION("const")
  {
    EiStr const o{"old"};
    EiStr const n{"new"};

    EiStr a = o;
    CHECK(*o == "old");
    CHECK(*a == "old");

    a = n;
    CHECK(*n == "new");
    CHECK(*a == "new");
  }

  SECTION("non-const")
  {
    EiStr o{"old"};
    EiStr n{"new"};

    EiStr a = o;
    CHECK(*o == "old");
    CHECK(*a == "old");

    a = n;
    CHECK(*n == "new");
    CHECK(*a == "new");
  }
}


TEST_CASE("ExplicitInit: moving")
{
  SECTION("const")
  {
    EiStr const o{"old"};
    EiStr const n{"new"};

    EiStr a = std::move(o);
    CHECK(*o == "old");
    CHECK(*a == "old");

    a = std::move(n);
    CHECK(*n == "new");
    CHECK(*a == "new");
  }

  SECTION("non-const")
  {
    EiStr o{"old"};
    EiStr n{"new"};

    EiStr a = std::move(o);
    CHECK(*o == "");
    CHECK(*a == "old");

    a = std::move(n);
    CHECK(*n == "");
    CHECK(*a == "new");
  }
}


TEST_CASE("ExplicitInit: moving underlying object")
{
  SECTION("const c-tor")
  {
    std::string const n{"new"};
    EiStr       const o = std::move(n);

    CHECK(*o == "new");
    CHECK( n == "new");
  }

  SECTION("non-const c-tor")
  {
    std::string n{"new"};
    EiStr       o = std::move(n);

    CHECK( n == "");
    CHECK(*o == "new");
  }

  SECTION("non-const assignment")
  {
    std::string n{"new"};
    EiStr       o{"old"};

    o = std::move(n);
    CHECK( n == "");
    CHECK(*o == "new");
  }
}

}
