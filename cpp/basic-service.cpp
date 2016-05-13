#include "basic-service.hpp"

namespace is {

using namespace AmqpClient;

BasicService::BasicService(Channel::ptr_t chann, const std::string& service) 
  : channel(chann), queue("service-" + service), exchange("services-basic") {    

  // passive durable exclusive auto_delete 
  channel->DeclareQueue(queue, false, true, false, false);
  
  // passive durable auto_delete 
  auto xtype = Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);

  std::vector<std::string> keys { service + ".*", "info" };
  for (auto& key : keys) {
    channel->BindQueue(queue, exchange, key);
  }

  channel->BasicConsume(queue);
}

void BasicService::bind(const std::string& key, handle_t handle) {
  map.emplace(key, handle);
}

void BasicService::process() {
  Envelope::ptr_t envelope;
  do {
    envelope = channel->BasicConsumeMessage();
    request = envelope->Message();  
  } while (
    request->ContentType() != "application/json" ||
    request->CorrelationIdIsSet() == false       ||
    request->ReplyToIsSet() == false
  );
  
  auto key = envelope->RoutingKey();
  auto handle = map.find(key);
  if (handle != map.end()) {
    handle->second(*this);
  } 
}

std::string BasicService::body() const {
  return request->Body();
}

void BasicService::reply(const std::string& body) const {
  BasicMessage::ptr_t response = BasicMessage::Create();
  response->CorrelationId(request->CorrelationId());
  response->Body(body);
  channel->BasicPublish(exchange, request->ReplyTo(), response);
}

} // ::is