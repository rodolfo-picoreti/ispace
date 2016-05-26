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

#include "../lib/publisher.hpp"
#include "../lib/broker.hpp"

typedef std::tuple <int, int, int, std::vector<unsigned char>> payload_t;

void usage() {
  std::cout << "pub-image <service-name> \n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) usage();

  cv::VideoCapture capture { 0 }; // open the default camera
  if (!capture.isOpened()) {
    std::cout << "Failed to open capture device" << std::endl;
  }

  is::Broker broker(argv[1]);
  broker.discover();

  is::Publisher publisher(broker.channel(), "data", "webcam");
  
  std::cout << "Publishing... " << std::endl;
  while (1) {
    cv::Mat frame;
    capture >> frame;

    std::vector<unsigned char> raw(frame.datastart, frame.dataend);
    payload_t payload {
      frame.rows, frame.cols, frame.type(), raw
    }; 

    publisher.publish(payload);
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  return 0;
}