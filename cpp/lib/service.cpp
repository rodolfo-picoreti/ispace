#include "service.hpp"

namespace is {

using namespace AmqpClient;

Service::Service(Channel::ptr_t channel, const std::string& service) 
  : channel(channel), queue("service-" + service), 
    exchange("services"), running(false) {    

  // passive durable exclusive auto_delete 
  channel->DeclareQueue(queue, false, true, false, false);
  
  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);

  std::vector<std::string> keys { service + ".*", "info" };
  for (auto& key : keys) {
    channel->BindQueue(queue, exchange, key);
  }
  
  //consumer_tag no_local no_ack exclusive
  channel->BasicConsume(queue, "", true, true, false);
}

Service::~Service() {
  if (running.load()) {
    stop();
    listener.join();
  }
}

void Service::bind(const std::string& key, handle_t handle) {
  if (running.load() == false) {
    map.emplace(key, handle);
  }
}

void Service::process() {
  Envelope::ptr_t envelope;

  while (1) {
    envelope = channel->BasicConsumeMessage();
    request = envelope->Message();  
    
    if (request->ContentType() == "application/msgpack" &&
        request->CorrelationIdIsSet() == true           &&
        request->ReplyToIsSet() == true) {
      break;
    } 
    else {
      // Invalid request
    }
  }
  
  auto key = envelope->RoutingKey();
  auto handle = map.find(key);
  if (handle != map.end()) {
    handle->second(*this);
  } 
}

void Service::stop() {
  running.store(false);
}

void Service::listen() {
  running.store(true);

  listener = std::thread([&]() {
    while (running.load()) {
      this->process();
    }
    return 0;
  });
}

void Service::listen_sync() {
  while (1) {
    this->process();  
  }
}

} // ::is