#include "JsonConsole.hpp"
#include "But/Log/Proxy.hpp"
#include "But/Log/Field/Timestamp.hpp"
#include "But/Log/Field/LineNumber.hpp"
#include "But/Log/Field/Priority.hpp"
#include "But/Log/Field/FileName.hpp"

using namespace But::Log::Field;
using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;


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
  But::Log::Destination::JsonConsole c;
  c.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  c.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );
  c.log( FormattedString{"format test"}, "whatever..." );
  c.flush();
  c.reload();

  But::Log::Destination::Sink& base = c;
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" );
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" );
  base.flush();
  base.reload();

  But::Log::Proxy<> lp{ But::makeSharedNN<But::Log::Destination::JsonConsole>() };
  lp.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  lp.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );
  lp.log( BUT_FORMAT("format test - $0"), "whatever..." );
  lp.log( "simple structure", Point{1,2} );
  lp.log( "nested structure", Line{ Point{3,4}, Point{9,8} } );
}
