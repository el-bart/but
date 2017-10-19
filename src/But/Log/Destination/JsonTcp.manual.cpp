#include <But/Log/Destination/JsonTcp.hpp>
#include <But/Log/Proxy.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using namespace But::Log::Field;
using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::detail::args2FieldInfo;


struct Point
{
  int x_;
  int y_;
};

auto toFieldInfo(Point const& p)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Point"}, { toFieldInfo(p.x_).retag(Tag{"x"}), toFieldInfo(p.y_).retag(Tag{"y"}) } };
}

struct Line
{
  Point from_;
  Point to_;
};

auto toFieldInfo(Line const& l)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Line"}, { toFieldInfo(l.from_).retag(Tag{"from"}), toFieldInfo(l.to_).retag(Tag{"to"}) } };
}


int main()
{
  auto tcp = But::makeSharedNN<But::Log::Destination::JsonTcp>("127.0.0.1", 4242);
  tcp->log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" ) );
  tcp->log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" ) );
  tcp->log( args2FieldInfo( FormattedString{"format test"}, "whatever..." ) );
  tcp->flush();
  tcp->reload();

  But::NotNullShared<But::Log::Destination::Sink> base{tcp};
  base->log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" ) );
  base->log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" ) );
  base->flush();
  base->reload();

  But::Log::Proxy<> lp{tcp};
  lp.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  lp.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );
  lp.log( BUT_FORMAT("format test - $0"), "whatever..." );
  lp.log( "simple structure", Point{1,2} );
  lp.log( "nested structure", Line{ Point{3,4}, Point{9,8} } );
}
