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
  
  static auto latency(BasicMessage::ptr_t message) {
    using namespace std::chrono;

    auto now = system_clock::now().time_since_epoch().count();
    auto diff = nanoseconds(now - message->Timestamp());
    return duration_cast<milliseconds>(diff).count();
  }

public:

  template <typename Payload>
  BasicMessage::ptr_t consume(Payload& payload) {
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

    return std::move(message);
  }
}; // ::Subscriber

} // ::is

#endif // __IS_SUBSCRIBRER_HPP__