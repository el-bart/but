#include <But/Log/Destination/VerboseTextConsole.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::detail::args2FieldInfo;
using Depth = But::Log::Destination::Common::VerboseTextStream::Depth;
using namespace But::Log::Field;

struct Nested
{
  int value_;
};

inline auto toFieldInfo(Nested n)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Nested"}, { toFieldInfo(n.value_) } };
}


struct DeeplyNested
{
  Nested nested_;
};

inline auto toFieldInfo(DeeplyNested dn)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"DeeplyNested"}, { toFieldInfo(dn.nested_) } };
}


int main()
{
  const auto nested = Nested{42};
  const auto deeplyNested = DeeplyNested{ nested };

  But::Log::Destination::VerboseTextConsole c{ Depth{1} };
  c.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "full", nested ) );
  c.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "partial", deeplyNested ) );
  c.log( args2FieldInfo( FormattedString{"format test"}, "whatever..." ) );
  c.flush();
  c.reload();
}
