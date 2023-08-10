#include <vector>
#include <But/Log/Destination/Sink.hpp>
#include <nlohmann/json.hpp>

namespace But::Log::Destination
{

/** @brief fake sink for UT purposes.
 */
struct FakeSink final: public Sink
{
  void logImpl(std::string&& str) override
  {
    logs_.push_back( std::move(str) );
  }

  void reloadImpl() override { ++reloads_; }
  void flushImpl() override  { ++flushes_; }

  auto parse(size_t index) const
  {
    return nlohmann::json::parse( logs_.at(index) );
  }

  unsigned reloads_{0};
  unsigned flushes_{0};
  std::vector<std::string> logs_;
};

}
