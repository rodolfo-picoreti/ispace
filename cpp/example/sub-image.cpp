#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../lib/subscriber.hpp"
#include "../lib/broker.hpp"

typedef std::tuple <int, int, int, std::vector<unsigned char>> payload_t;
auto credentials = std::make_tuple("ispace", "ispace", "is");

void usage() {
  std::cout << "sub-image <service-name> <topic>\n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

int main(int argc, char const *argv[]) {
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

  // Create data subscriber
  is::Subscriber subscriber(channel, "data", argv[2]);

  double mean = 0;
  while (1) {
    payload_t payload;
    subscriber.consume(payload);
    auto latency = subscriber.latency();
    mean = mean*0.8 + latency*0.2;

    // print mean latency time
    std::cout << "\r        \r" << std::round(mean) << std::flush;

    int rows = std::get<0>(payload);
    int cols = std::get<1>(payload);
    int type = std::get<2>(payload);
    unsigned char* data = std::get<3>(payload).data();
    int size = std::get<3>(payload).size();

    cv::Mat image(rows, cols, type, data, size/rows);
    cv::imshow("Webcam stream", image);
    cv::waitKey(1);
  }     
}
