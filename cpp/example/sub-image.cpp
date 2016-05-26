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

void usage() {
  std::cout << "sub-image <service-name> \n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) usage();

  is::Broker broker(argv[1]);
  broker.discover();

  is::Subscriber subscriber(broker.channel(), "data", "webcam");
  
  while (1) {
    payload_t payload;
    subscriber.consume(payload);

    int rows = std::get<0>(payload);
    int cols = std::get<1>(payload);
    int type = std::get<2>(payload);
    unsigned char* data = std::get<3>(payload).data();

    cv::Mat image(rows, cols, type, data);
    cv::imshow("Received image", image);
    cv::waitKey(1);
  }     

}