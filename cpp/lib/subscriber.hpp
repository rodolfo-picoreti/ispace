#ifndef __IS_SUBSCRIBRER_HPP__
#define __IS_SUBSCRIBRER_HPP__

#include <string>
#include <future>
#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class Subscriber {

  Channel::ptr_t channel;
  const std::string exchange;
  const std::string key;
  
public:

  Subscriber(Channel::ptr_t channel, const std::string& exchange, const std::string& key);
  
  std::future<msgpack::object> consume() const;

}; // ::Subscriber

} // ::is

#endif // __IS_SUBSCRIBRER_HPP__