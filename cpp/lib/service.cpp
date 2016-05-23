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

void Service::bind(const std::string& key, handle_t handle) {
  map.emplace(key, handle);
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

std::string Service::body() const {
  return request->Body();
}

void Service::reply(const std::string& body) const {
  BasicMessage::ptr_t response = BasicMessage::Create();
  response->ContentType("application/msgpack");
  response->CorrelationId(request->CorrelationId());
  response->Body(body);
  channel->BasicPublish(exchange, request->ReplyTo(), response);
}

void Service::stop() {
  running.store(false);
}

std::thread Service::listen() {
  running.store(true);

  return std::thread([&]() {
    while (running.load()) {
      this->process();
    }
    return 0;
  });
}

} // ::is