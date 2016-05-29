#include "broker.hpp"

namespace is {
  
using namespace AmqpClient;

Broker::Broker() : avahi("_rabbitmq._tcp") { }

const std::vector<Avahi::Service>& Broker::discover(const std::string& hostname, int port) {
  return avahi.discover([&] (const is::Avahi::Service& service) {
    return (service.port == port && service.name == hostname);
  });
}

Channel::ptr_t Broker::connect(const Avahi::Service& service, const Credentials& credentials) {
  return AmqpClient::Channel::Create (
    service.ip, 
    service.port, 
    std::get<0>(credentials),
    std::get<1>(credentials),
    std::get<2>(credentials)
  ); 
}

} // ::is