#pragma once
#include <type_traits>
#include <unordered_map>
#include <cassert>
#include <boost/cast.hpp>
#include "But/Exception.hpp"
#include "But/Mpl/NamedVariadicTemplate.hpp"
#include "detail/Registrator.hpp"

namespace But
{
namespace Pattern
{

/** @brief base class for dispatching. it is parametrized with the binary format type
 *         (usually such type contains ID and serialized data).
 */
template<typename B>
struct Dispatcher
{
  BUT_DEFINE_EXCEPTION(UnknownMessage, Exception, "unknown message");

  using BinaryFormat = B;

  Dispatcher() = default;
  Dispatcher(Dispatcher const&) = delete;
  Dispatcher& operator=(Dispatcher const&) = delete;

  virtual ~Dispatcher() = default;
  virtual void dispatch(BinaryFormat const& bin) = 0;
};


/** @brief dispatcher pattern, that automatically deserializes incomming messages and direct them
 *         to the proper handle() message, implemented by a user in a FinalType derived class.
 *
 *  @par
 *  typical usage is to derive from this class, specify policy and list of messages to handle.
 *  messages are forwarded to hanle() methods of FinalType object.
 *
 *  @par
 *  Policy type need to specify ID and binary format types, methods for getting messages IDs and
 *  method to deserialize given binary data to a template-provided output type.
 *
 *  @par
 *  example usage is shown below.
 *
 *  <code>
 *    struct MyBinaryData
 *    {
 *      int         id_;
 *      std::string data_;
 *    };
 *
 *    struct MyPolicy
 *    {
 *      using IdType       = int;                     // type of message identifiers
 *      using BinaryFormat = MyBinaryData;            // serialized format of data
 *
 *      template<typename M>
 *      static constexpr IdType getId();              // compile-time method for getting IdType for a given message type M.
 *
 *      static IdType getId(BinaryFormat const& bin); // run-time method for extracting IdType from serialized message.
 *
 *      template<typename M>
 *      static M deserialize(BinaryFormat const& bin);// method performing deserialization of serialized message into a given type M
 *    };
 *
 *    struct MyDispatcher final: public AutoDispatcher<MyDispatcher, MyPolicy,
 *                                                     Msg1, Msg2, Msg3>
 *    {
 *      void handle(Msg1 const&) { your reaction on a given message }
 *      void handle(Msg2 const&) { your reaction on a given message }
 *      void handle(Msg3 const&) { your reaction on a given message }
 *    };
 *
 *    void myProcessingLoop(Dispatcher<MyBinaryData>& d)    // note the usage of the base class here!
 *    {
 *      while(hasNewData())
 *      {
 *        MyBinaryData bin = receiveData();
 *        d.dispatch(bin);
 *      }
 *    }
 *
 *    int main()
 *    {
 *      MyDispatcher md;
 *      myProcessingLoop(md);
 *    }
 *  </code>
 */
template<typename FinalType, typename Policy, typename ...Msgs>
struct AutoDispatcher: public Dispatcher<typename Policy::BinaryFormat>,
                       public Policy
{
public:
  using PolicyType   = Policy;
  using IdType       = typename Policy::IdType;
  using BinaryFormat = typename Policy::BinaryFormat;
  using Base         = Dispatcher<BinaryFormat>;
  using Messages     = Mpl::NamedVariadicTemplate<Msgs...>;

  AutoDispatcher()
  {
    detail::Registrator<FinalType, Msgs..., void>::call(handlers_);
    assert( sizeof...(Msgs) == handlers_.size() && "non-unique messages IDs detected" );
  }

  virtual void dispatch(typename Policy::BinaryFormat const& bin) override final
  {
    static_assert( std::is_final<FinalType>::value, "FinalType is not the last one in a derive chain - mark FinalType type as 'final'" );
    const auto id = Policy::getId(bin);
    const auto it = handlers_.find(id);
    if(it == end(handlers_))
      BUT_THROW(typename Base::UnknownMessage, id);
    const auto h = it->second;
    assert(h && "handler is not set - implementation bug");
    (*h)( *boost::polymorphic_downcast<FinalType*>(this), bin );
  }

private:
  using Handler = void (*)(FinalType&, BinaryFormat const&);

  std::unordered_map<IdType, Handler> handlers_;
};

}
}
