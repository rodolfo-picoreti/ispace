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

public:

  template <typename Payload>
  void consume(Payload& payload) {
    Envelope::ptr_t envelope;
    BasicMessage::ptr_t message; 
    
    do {
      envelope = channel->BasicConsumeMessage();
      message = envelope->Message(); 
    } while (message->ContentType() != "application/msgpack");

    std::string body(message->Body());
    msgpack::object_handle handle = msgpack::unpack(body.data(), body.size());
    
    // object is valid during the object_handle instance is alive.
    msgpack::object object = handle.get();
    object.convert(payload);
  }
}; // ::Subscriber

} // ::is

#endif // __IS_SUBSCRIBRER_HPP__