#include <iostream>
#include <string>
#include <chrono>
#include <tuple>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "../lib/client.hpp"
#include "../lib/broker.hpp"

auto credentials = std::make_tuple("ispace", "ispace", "is");

void usage() {
  std::cout << "service-client <service-name> <fps>\n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

int main(int argc, char* argv[]) {
  if (argc != 3) usage();
  
  // Discover and connect to broker
  std::cout << "Searching for broker... " << std::flush;
  is::Broker broker;
  auto found = broker.discover(argv[1]); // list of brokers found
  if (found.empty()) {
    std::cout << "[error]" << std::endl;
    exit(-1);
  } else {
    std::cout << "[ok]" << std::endl;
  }

  auto channel = broker.connect(found.front(), credentials); // connect to the first
  
  is::Client client(channel, "webcam.fps");
  
  std::tuple<double> args { std::stod(argv[2]) };
  auto id = client.request(args);
  auto ok = client.receive(id, 2000);
  if (ok) {
    std::tuple<std::string> reply;
    client.consume(id, reply);
    std::cout << "Received: " << std::get<0>(reply) << std::endl;
  } 
  else {
    std::cout << "Request failed due to timeout :(" << std::endl;
  }

} 
