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

  BasicMessage::ptr_t message; 

public:

  Subscriber(Channel::ptr_t channel, const std::string& exchange, const std::string& key);
  
  unsigned int latency();

public:

  template <typename Payload>
  bool consume(Payload& payload, int timeout = -1) {
    Envelope::ptr_t envelope;
    
    do {
      bool newmsg = channel->BasicConsumeMessage(envelope, timeout);
      if (newmsg == false) {
        // timeout reached and nothing was received
        return false;
      }
      message = envelope->Message(); 
    } while (message->ContentType() != "application/msgpack" ||
             message->TimestampIsSet() == false);

    std::string body(message->Body());
    msgpack::object_handle handle = msgpack::unpack(body.data(), body.size());
    
    // object is valid during the object_handle instance is alive.
    msgpack::object object = handle.get();
    object.convert(payload);

    return true;
  }

}; // ::Subscriber

} // ::is

#endif // __IS_SUBSCRIBRER_HPP__