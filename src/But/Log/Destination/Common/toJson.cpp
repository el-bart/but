#include <cassert>
#include <boost/lexical_cast.hpp>
#include "toJson.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

namespace
{
struct ValueVisitor final
{
  template<typename T>
  void operator()(T const& t)
  {
    assert(type_);
    assert(field_);
    (*field_)[ type_->str() ] = t;
  }

  Backend::Type const* type_{nullptr};
  json* field_{nullptr};
};


struct NestedFieldInfoVisitor final
{
  void operator()(Backend::Type const& t, Backend::Value const& v)
  {
    ValueVisitor vv{&t, &field_};
    v.visit(vv);
  }
  void operator()(Backend::Type const& t, std::vector<Backend::FieldInfo>const& fis)
  {
    NestedFieldInfoVisitor fiv;
    for(auto& e: fis)
      e.visit(fiv);
    field_[ t.str() ] = std::move(fiv.field_);
  }

  json field_;
};

struct FieldInfoVisitor final
{
  void operator()(Backend::Type const& t, Backend::Value const& v)
  {
    json tmp;
    ValueVisitor vv{&t, &tmp};
    v.visit(vv);
    field_.push_back( std::move(tmp) );
  }
  void operator()(Backend::Type const& /*t*/, std::vector<Backend::FieldInfo>const& fis)
  {
    for(auto& e: fis)
    {
      NestedFieldInfoVisitor fiv;
      e.visit(fiv);
      field_.push_back( std::move(fiv.field_) );
    }
  }

  json field_ = json::array();
};
}


nlohmann::json toJson(Backend::FieldInfo const& fi)
{
  FieldInfoVisitor fiv;
  fi.visit(fiv);
  return std::move(fiv.field_);
}

}
}
}
}
