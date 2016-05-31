#include "subscriber.hpp"

namespace is {

using namespace AmqpClient;

Subscriber::Subscriber(Channel::ptr_t channel, 
                       const std::string& exchange, 
                       const std::string& key) 
  : channel(channel), exchange(exchange), key(key), message(nullptr) {
  
  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);

  std::string queue = channel->DeclareQueue("");
  channel->BindQueue(queue, exchange, key);
  channel->BasicConsume(queue);
}

unsigned int Subscriber::latency() {
  using namespace std::chrono;

  auto now = system_clock::now().time_since_epoch().count();
  auto diff = nanoseconds(now - message->Timestamp());
  return duration_cast<milliseconds>(diff).count();
}

} // ::is