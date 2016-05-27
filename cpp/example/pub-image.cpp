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

  std::cout << "Starting camera... " << std::endl;
  cv::VideoCapture capture { 0 }; // open the default camera
  if (!capture.isOpened()) {
    std::cout << "Failed to open capture device" << std::endl;
  }

  std::cout << "Searching for broker... " << std::endl;
  is::Broker broker(argv[1]);
  broker.discover();

  is::Publisher publisher(broker.channel(), "data", "webcam");
  

  double fps = 30.0;
  auto delta = std::chrono::milliseconds(static_cast<int>(std::round(1000.0/fps)));

  unsigned int count = 0;
  cv::Mat frame;
  capture >> frame;
  std::cout << "Publishing... " << frame.rows << 'x' << frame.cols 
            << ' ' << (frame.dataend - frame.datastart)*fps/1024/1024 << " MB/s" << std::endl;

  while (1) {
    auto now = std::chrono::system_clock::now();
    capture >> frame;

    std::vector<unsigned char> raw(frame.datastart, frame.dataend);
    payload_t payload {
      frame.rows, frame.cols, frame.type(), raw
    }; 

    publisher.publish(payload);

    std::cout << '\r' << count << std::flush;
    ++count;
    
    std::this_thread::sleep_until(now + delta);
  }

  return 0;
}