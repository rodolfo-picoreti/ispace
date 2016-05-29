#ifndef __IS_PUBLISHER_HPP__
#define __IS_PUBLISHER_HPP__

#include <string>
#include <thread>
#include <sstream>
#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class Publisher {

  Channel::ptr_t channel;
  const std::string exchange;
  const std::string key;
  
public:

  Publisher(Channel::ptr_t channel, const std::string& exchange, const std::string& key);
  
  template <typename Payload>
  void publish(const Payload& payload) const {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::stringstream body;
    msgpack::pack(body, payload);
    auto message = BasicMessage::Create(body.str());
    message->ContentType("application/msgpack");
    message->Timestamp(timestamp); // nanoseconds
    channel->BasicPublish(exchange, key, message);
  }

}; // ::Publisher

} // ::is

#endif // __IS_PUBLISHER_HPP__