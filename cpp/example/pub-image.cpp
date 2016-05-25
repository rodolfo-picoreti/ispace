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
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "../lib/publisher.hpp"

const std::string host { "127.0.0.1" };
int port { 5672 };
const std::string username { "ispace" };
const std::string password { "ispace" };
const std::string vhost { "is" };

typedef std::tuple<int, int, int, std::vector<unsigned char>> payload_t;

int main(int argc, char const *argv[]) {
  
  cv::VideoCapture capture { 0 }; // open the default camera
  if (!capture.isOpened()) {
    std::cout << "Failed to open capture device" << std::endl;
  }

  auto channel = AmqpClient::Channel::Create(host, port, username, password, vhost); 
  is::Publisher publisher(channel, "data", "webcam");
  
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