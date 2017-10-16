#include <gtest/gtest.h>
#include <But/Log/Localization/Translation.hpp>
#include <But/Format/format.hpp>

using But::Log::Localization::Translation;

namespace
{

struct ButLogLocalizationTranslation: public testing::Test
{
  Translation::Data data_{
                           { {"in1"},           {"out1"} },
                           { {"in2 $0"},        {"out2 $0"} },
                           { {"in3 / ${0} $1"}, {"out3 / $1 $0"} },
                           { {"$0 $1 $2"},      {"$2 $2 $1 $0"} }
                         };
  const Translation tr_{data_};
};


TEST_F(ButLogLocalizationTranslation, TranslatingDefinedFormats)
{
  {
    const auto local = tr_.translate( BUT_FORMAT("in1") );
    EXPECT_EQ( 0u, local.expectedArguments() );
    EXPECT_EQ( std::string{"out1"}, local.inputFormat() );
  }
  {
    const auto local = tr_.translate( BUT_FORMAT("in2 $0") );
    EXPECT_EQ( 1u, local.expectedArguments() );
    EXPECT_EQ( std::string{"out2 $0"}, local.inputFormat() );
  }
  {
    const auto local = tr_.translate( BUT_FORMAT("in3 / ${0} $1") );
    EXPECT_EQ( 2u, local.expectedArguments() );
    EXPECT_EQ( std::string{"out3 / $1 $0"}, local.inputFormat() );
  }
  {
    const auto local = tr_.translate( BUT_FORMAT("$0 $1 $2") );
    EXPECT_EQ( 3u, local.expectedArguments() );
    EXPECT_EQ( std::string{"$2 $2 $1 $0"}, local.inputFormat() );
  }
}


TEST_F(ButLogLocalizationTranslation, UnefinedFormatsAreNotTranslated)
{
  auto fmt = BUT_FORMAT("unknown format");
  const auto local = tr_.translate( std::move(fmt) );
  EXPECT_EQ( std::string{"unknown format"}, local.inputFormat() );
}


TEST_F(ButLogLocalizationTranslation, ErrorsOnFormatMissmatchThrow)
{
  EXPECT_NO_THROW( Translation{data_} );
  data_.push_back( Translation::Entry{ {"sth in"}, {"other out $0"} } );
  EXPECT_THROW( Translation{data_}, Translation::IncompatibleFormats );
}

}
