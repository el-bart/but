#include <But/Log/DefaultLogger.hpp>
#include <But/detail/catch2.hpp>
#include <But/Log/Field/tag.hpp>
#include <But/Log/Destination/FakeSink.ut.hpp>

using But::Log::DefaultLogger;
using But::Log::makeDefaultLogger;
using But::Log::makeDefaultConsoleLogger;
using But::Log::Destination::FakeSink;
using But::Log::Field::tag;

namespace
{

SCENARIO("DefaultLogger's functional tests")
{
  auto const sink = But::makeSharedNN<FakeSink>();
  DefaultLogger logger{sink};

  WHEN("no logs logged")
  {
    THEN("output is empty")
    {
      CHECK( sink->size() == 0u );
    }
  }

  WHEN("logging 4 messages")
  {
    logger.debug("test debug", tag("foo", "bar1"));
    logger.info("test info", tag("foo", "bar2"));
    logger.warning("test warning", tag("foo", "bar3"));
    logger.error("test error", tag("foo", "bar4"));

    THEN("logs are captured")
    {
      REQUIRE( sink->size() == 4u );

      AND_THEN("debug log matches")
      {
        auto const l = sink->parse(0);
        INFO(l);
        CHECK( l["message"] == "test debug" );
        CHECK( l["But::LogLevel"] == "debug" );
        CHECK( l["But::Pid"] == getpid() );
        CHECK( l.contains("But::PreciseDT") );
        CHECK( l.contains("But::ThreadId") );
        CHECK( l["foo"] == "bar1" );
      }

      AND_THEN("info log matches")
      {
        auto const l = sink->parse(1);
        INFO(l);
        CHECK( l["message"] == "test info" );
        CHECK( l["But::LogLevel"] == "info" );
        CHECK( l["But::Pid"] == getpid() );
        CHECK( l.contains("But::PreciseDT") );
        CHECK( l.contains("But::ThreadId") );
        CHECK( l["foo"] == "bar2" );
      }

      AND_THEN("warning log matches")
      {
        auto const l = sink->parse(2);
        INFO(l);
        CHECK( l["message"] == "test warning" );
        CHECK( l["But::LogLevel"] == "warning" );
        CHECK( l["But::Pid"] == getpid() );
        CHECK( l.contains("But::PreciseDT") );
        CHECK( l.contains("But::ThreadId") );
        CHECK( l["foo"] == "bar3" );
      }

      AND_THEN("error log matches")
      {
        auto const l = sink->parse(3);
        INFO(l);
        CHECK( l["message"] == "test error" );
        CHECK( l["But::LogLevel"] == "error" );
        CHECK( l["But::Pid"] == getpid() );
        CHECK( l.contains("But::PreciseDT") );
        CHECK( l.contains("But::ThreadId") );
        CHECK( l["foo"] == "bar4" );
      }
    }
  }

  WHEN("added common field")
  {
    auto more = logger.withFields( tag("answer", 42) );

    AND_WHEN("logging")
    {
      more.info("whatever", tag("foo", "bar"));

      THEN("log is captured with all the fields")
      {
        auto const l = sink->parse(0);
        INFO(l);
        CHECK( l["message"] == "whatever" );
        CHECK( l["But::LogLevel"] == "info" );
        CHECK( l["But::Pid"] == getpid() );
        CHECK( l.contains("But::PreciseDT") );
        CHECK( l.contains("But::ThreadId") );
        CHECK( l["answer"] == 42 );
        CHECK( l["foo"] == "bar" );
      }
    }
  }
}


TEST_CASE("making DefaultLogger with helpers")
{
  {
    auto logger = makeDefaultLogger<FakeSink>();
    logger.info("test");
  }

  {
    auto logger = makeDefaultConsoleLogger();
    logger.reload();
  }

  {
    auto logger = makeDefaultConsoleLogger(false);
    logger.reload();
  }
}

}
