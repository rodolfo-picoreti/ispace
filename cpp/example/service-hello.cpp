#include <iostream>
#include <string>
#include <chrono>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "../lib/service.hpp"
#include "../lib/broker.hpp"

void hello(is::Service& service) {
  std::tuple<std::string> name;
  service.args(name);
  std::cout << " [x] Received: " << std::get<0>(name) << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  
  std::get<0>(name) = "Hello " + std::get<0>(name);
  service.reply(name);    
  std::cout << " [x] Sent: " << std::get<0>(name) << std::endl;
}

void usage() {
  std::cout << "service-hello <service-name> \n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

int main(int argc, char* argv[]) {
  if (argc != 2) usage();

  is::Broker broker(argv[1]);
  broker.discover();

  is::Service service(broker.channel(), "hello");
  service.bind("hello.exec", hello);

  auto bg = service.listen();
  bg.join();
}
