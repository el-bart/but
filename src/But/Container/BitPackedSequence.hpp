#pragma once
#include <vector>
#include <limits>

namespace But
{
namespace Container
{

/** @brief container-adapter for keeping data bit-wise (as an oposite to typical byte-wise).
 *
 *  @warning writing to different elements of the same container may be a data race. just don't do that.
 *
 * TODO: finish description
 *
 * TODO: provide usage example
 */
template<typename T, typename Packer, typename Container = std::vector<uint8_t>>
class BitPackedSequence final
{
  static_assert( Packer::bits_count <= 8u,
                 "for now up to 8-bits packed values are supported" );
  static_assert( (1<<(Packer::bits_count-1u)) <= std::numeric_limits<typename Container::value_type>::max(),
                 "packer produces elements that are bigger than container's single element capacity" );

public:
  using size_type  = typename Container::size_type;
  using value_type = T;
  using iterator       = void*; // TODO
  using const_iterator = void*; // TODO

  bool empty() const { return c_.empty(); }
  size_type size() const { return c_.size(); }

  iterator begin() { return nullptr; } // TODO
  iterator end()   { return nullptr; } // TODO

  const_iterator begin() const { return nullptr; } // TODO
  const_iterator end()   const { return nullptr; } // TODO
  const_iterator cbegin() const { return begin(); }
  const_iterator cend()   const { return end(); }

  void push_back(const value_type v)
  {
    const auto bits = Packer::encode(v);
    // TODO...
    c_.push_back(bits);
  }

  value_type operator[](const size_type pos) const
  {
    // TODO...
    const auto e = Packer::decode( c_[pos] );
    return e;
  }

private:
  Container c_;
};

}
}
