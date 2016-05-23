#include "subscriber.hpp"

namespace is {

using namespace AmqpClient;

Subscriber::Subscriber(Channel::ptr_t channel, 
                       const std::string& exchange, 
                       const std::string& key) 
  : channel(channel), exchange(exchange), key(key) {
  
  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);

  std::string queue = channel->DeclareQueue("");
  channel->BindQueue(queue, exchange, key);
  channel->BasicConsume(queue);
}

std::future<msgpack::object> Subscriber::consume() const {
  return std::async(std::launch::async, [] (Channel::ptr_t channel) { 
    Envelope::ptr_t envelope;
    BasicMessage::ptr_t message; 
    
    do {
      envelope = channel->BasicConsumeMessage();
      message = envelope->Message(); 
    } while (message->ContentType() != "application/msgpack");

    auto body = message->Body();
    std::size_t offset = 0;
    auto unpacked = msgpack::unpack(body.data(), body.size(), offset);
    auto payload = unpacked.get();
    return payload;  
  }, channel);
}

} // ::is