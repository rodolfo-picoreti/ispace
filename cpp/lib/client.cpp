#include "client.hpp"

namespace is {

using namespace AmqpClient;

Client::Client(Channel::ptr_t channel, const std::string& key)
  : channel(channel), exchange("services"), key(key), correlationId(0) {

  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  this->channel->DeclareExchange(exchange, xtype, false, true, false);

  // Create queue to receive rpc responses
  queue = this->channel->DeclareQueue("");
  this->channel->BindQueue(queue, exchange, queue);
  this->channel->BasicConsume(queue);

}

bool Client::receive(const uint& id, const uint& timeout) {
  using namespace std::chrono;
  auto end = high_resolution_clock::now() + milliseconds(timeout);
  
  while (1) {  
    Envelope::ptr_t envelope;
    BasicMessage::ptr_t message; 

    do {  
      auto now = high_resolution_clock::now();
      int delta = duration_cast<milliseconds>(end-now).count();
      if (delta <= 0) {
        return false; // we are out of time already, leave!
      }

      bool newmsg = channel->BasicConsumeMessage(envelope, delta);
      if (newmsg == false) {
        // timeout reached and nothing was received
        return false;
      }

      message = envelope->Message(); 

    } while (message->ContentType() != "application/msgpack" ||
             message->CorrelationIdIsSet() == false);
      
    uint cid = std::stoi(message->CorrelationId());
    map.emplace(cid, std::move(message));

    if (cid == id) {
      // packet arrived, return
      return true;
    }
  }
}

} // ::is