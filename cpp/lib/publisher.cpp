#include "publisher.hpp"

namespace is {

using namespace AmqpClient;

Publisher::Publisher(Channel::ptr_t channel, 
                     const std::string& exchange, 
                     const std::string& key)
  : channel(channel), exchange(exchange), key(key) {

  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);
}

} // ::is