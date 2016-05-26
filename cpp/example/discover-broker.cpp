#include "../lib/avahi.hpp"

int main(int argc, char*argv[]) {
  is::Avahi avahi("_rabbitmq._tcp", [] (const is::Avahi::Service& service) {
    return (service.port == 5672);
  });
  
  avahi.discover();

  for (auto& service : avahi.discovered()) {
    std::cout << "name: " << service.name << '\n'
              << "domain: " << service.domain << '\n'
              << "host: " << service.host << '\n'
              << "type: " << service.type << '\n'
              << "ip: " << service.ip << '\n'
              << "port: " << service.port << std::endl;
  }

  return 0;
}