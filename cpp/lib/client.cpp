#include "client.hpp"

namespace is {

using namespace AmqpClient;

Client::Client(Channel::ptr_t channel, const std::string& key)
  : channel(channel), exchange("services"), key(key), running(false) {

  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);

  // Create queue to receive rpc responses
  queue = channel->DeclareQueue("");
  channel->BindQueue(queue, exchange, queue);
  channel->BasicConsume(queue);

  // Start background listener thread if not running already
  //spawn();
}

Client::~Client() {
  running.store(false);
  listener.join();
}

/*
void Client::receive() {
  std::string body(message->Body());
  msgpack::object_handle handle = msgpack::unpack(body.data(), body.size());
    
    
  Envelope::ptr_t envelope;
  BasicMessage::ptr_t message; 
  int timeout { 100 };

  do {
    newmsg = channel->BasicConsumeMessage(envelope, timeout);
    if (!newmsg) return;
    message = envelope->Message(); 
  } while (message->ContentType() != "application/msgpack" ||
           message->CorrelationIdIsSet() == false);

}

void Client::spawn() {
  if (!running.load()) {
    running.store(true);

    listener = std::thread([&]() {
      while (running.load()) {
        this->receive();
      }
      return 0;
    });
  }
}
*/
} // ::is