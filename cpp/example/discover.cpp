
#include "../lib/broker.hpp"

int main(int argc, char* argv[]) {

  // Discover and connect to broker
  std::cout << "Searching for broker... " << std::flush;
  is::Broker broker;
  auto services = broker.discover(argv[1]);
  if (services.empty()) {
    std::cout << "[error]" << std::endl;
    exit(-1);
  } else {
    std::cout << "[ok]" << std::endl;
  }

  for (auto& service : services) {
    std::cout << "name: " << service.name << '\n'
              << "domain: " << service.domain << '\n'
              << "host: " << service.host << '\n'
              << "type: " << service.type << '\n'
              << "ip: " << service.ip << '\n'
              << "port: " << service.port << std::endl;
  }
}