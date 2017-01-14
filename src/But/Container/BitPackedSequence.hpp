#pragma once
#include <vector>
#include <limits>
#include <type_traits>
#include "OffsetIterator.hpp"

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
  static_assert( std::is_pod<typename Container::value_type>::value, "array element must be POD" );

  using this_type = BitPackedSequence<T, Packer, Container>;

public:
  using size_type = typename Container::size_type;
  using value_type = T;
  using packer_type = Packer;
  using container_type = Container;
  using difference_type = typename container_type::difference_type;

  class BitProxy final
  {
  public:
    value_type operator=(value_type const& other)
    {
      assert(c_);
      c_->insertValueAtPosition(other, pos_);
      return other;
    }

    value_type operator=(BitProxy const& other)
    {
      return *this = static_cast<value_type>(other);
    }

    operator const value_type() const
    {
      assert(c_);
      return c_->readValueAtPosition(pos_);
    }

  private:
    BitProxy(this_type& c, const size_type pos):
      c_{&c},
      pos_{pos}
    { }

    friend this_type;

    this_type* c_;
    size_type pos_;
  };

  using iterator       = OffsetIterator<this_type, BitProxy>;
  using const_iterator = OffsetIterator<const this_type, value_type>;

  bool empty() const { return size() == 0u; }
  size_type size() const { return size_; }
  size_type capacity() const { return ( c_.size() * array_element_bits ) / Packer::bits_count; }


  auto begin() { return iterator{*this, 0}; }
  auto end()   { return iterator{*this, size()}; }

  auto begin() const { return const_iterator{*this, 0}; }
  auto end()   const { return const_iterator{*this, size()}; }
  auto cbegin() const { return begin(); }
  auto cend()   const { return end(); }

  void push_back(const value_type v)
  {
    resizeToFitAdditional(1);
    assert( capacity() > size() );
    insertValueAtPosition(v, size_);
    ++size_;
  }

  void erase(const const_iterator pos)
  {
    if( pos == end() )
      return;
    auto start = toNonConst(pos);
    for(auto it=start; it+1!=end(); ++it)
      *it = *(it+1);
    --size_;
    resizeToReducedSize();
  }

  BitProxy operator[](const size_type pos) { return BitProxy{*this, pos}; }
  value_type operator[](const size_type pos) const { return readValueAtPosition(pos); }

private:
  iterator toNonConst(const_iterator it)
  {
    auto n = std::distance( cbegin(), it );
    return begin() + n;
  }

  using array_element_type = typename Container::value_type;

  static constexpr array_element_type maskForFirstBits(const uint8_t count)
  {
    if(count==0u)
      return 0;
    return (1<<(count-1)) | maskForFirstBits(count-1);
  }

  static constexpr uint8_t bits_per_byte      = 8u;
  static constexpr uint8_t array_element_bits = sizeof(array_element_type) * bits_per_byte;
  static constexpr auto    array_element_mask = std::numeric_limits<array_element_type>::max();
  static constexpr auto    element_bits_mask  = maskForFirstBits(Packer::bits_count);

  void resizeToFitAdditional(const size_type additional)
  {
    while( capacity() < size() + additional )
      c_.push_back({});
  }
  void resizeToReducedSize()
  {
    const auto bitsAvailable = c_.size() * bits_per_byte;
    const auto bitsRequired = size_ * Packer::bits_count;
    assert( bitsRequired <= bitsAvailable );
    if( bitsAvailable - bitsRequired >= bits_per_byte )
      c_.pop_back();
  }

  void insertValueAtPosition(const value_type v, const size_type pos)
  {
    const auto bits = Packer::encode(v);

    const auto startByte = (pos * Packer::bits_count) / array_element_bits;
    const auto startOffset = pos * Packer::bits_count - startByte * array_element_bits;
    const auto bitsLeftInStartByte = array_element_bits - startOffset;
    assert( bitsLeftInStartByte > 0u && "wtf?!" );
    const auto shiftForStart = array_element_bits - bitsLeftInStartByte;
    const auto maskForStart = ~ ( element_bits_mask << shiftForStart );
    c_[startByte] &= maskForStart;
    c_[startByte] |= bits << shiftForStart;

    if( bitsLeftInStartByte < Packer::bits_count )
    {
      const auto endByte = startByte + 1u;
      c_[endByte] &= ~ ( element_bits_mask >> bitsLeftInStartByte );
      c_[endByte] |= bits >> bitsLeftInStartByte;
    }
  }

  value_type readValueAtPosition(const size_type pos) const
  {
    const auto startByte = (pos * Packer::bits_count) / array_element_bits;
    const auto startOffset = pos * Packer::bits_count - startByte * array_element_bits;
    const auto bitsLeftInStartByte = array_element_bits - startOffset;
    assert( bitsLeftInStartByte > 0u && "wtf?!" );
    const auto shiftForStart = array_element_bits - bitsLeftInStartByte;
    const auto maskForStart = element_bits_mask << shiftForStart;
    const auto firstByte = c_[startByte] & maskForStart;
    const auto firstPart = firstByte >> startOffset;
    if( bits_per_byte < shiftForStart + Packer::bits_count )
    {
      const auto endByte = startByte+1;
      assert( endByte < size() );
      const auto lastByte = c_[endByte];
      const auto bitsLeft = ( shiftForStart + Packer::bits_count ) - bits_per_byte;
      const auto maskForEnd = element_bits_mask >> ( Packer::bits_count - bitsLeft );
      const auto lastPart = lastByte & maskForEnd;
      const auto data = firstPart | ( lastPart << bitsLeftInStartByte );
      return Packer::decode(data);
    }
    return Packer::decode(firstPart);
  }


  friend class BitProxy;

  size_type size_{0};
  Container c_;
};

}
}
