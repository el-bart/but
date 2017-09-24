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
struct FieldAddVisitor final
{
  template<typename T>
  void operator()(T const& t)
  {
    assert(field_);
    assert(type_);
    (*field_)[ type_->str() ] = t;
  }

  Backend::Type const* type_;
  json* field_;
};
}


nlohmann::json toJsonField(Backend::FieldInfo const& fi)
{
  json field;
  FieldAddVisitor fav{ &fi.type(), &field };
  fi.value().visit(fav);
  return field;
}


void toJson(json& out, Backend::Entry const& entry)
{
  for(auto& e: entry)
    out.push_back( toJsonField(e) );
}


nlohmann::json toJson(Backend::Entry const& entry)
{
  auto out = nlohmann::json::array();
  toJson(out, entry);
  return out;
}

}
}
}
}
