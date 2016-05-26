#include "broker.hpp"

namespace is {
  
using namespace AmqpClient;

Broker::Broker(const std::string& hostname, int port, 
  const std::string& username, const std::string& password, const std::string& vhost) : 
hostname(hostname), port(port), username(username), password(password), vhost(vhost) {}

void Broker::discover() {
  is::Avahi avahi("_rabbitmq._tcp", [&] (const is::Avahi::Service& service) {
    return (service.port == port && service.name == hostname);
  });
  
  avahi.discover();
  auto services = avahi.discovered();
  _channel = AmqpClient::Channel::Create(services.at(0).ip, port, username, password, vhost); 
}

Channel::ptr_t Broker::channel() {
  return _channel;
}


} // ::is