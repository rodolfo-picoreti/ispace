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

#include "../lib/subscriber.hpp"

const std::string host { "127.0.0.1" };
int port { 5672 };
const std::string username { "ispace" };
const std::string password { "ispace" };
const std::string vhost { "is" };

typedef std::tuple<int, int, int, std::vector<unsigned char>> payload_t;

int main(int argc, char const *argv[]) {

  auto channel = AmqpClient::Channel::Create(host, port, username, password, vhost); 
  is::Subscriber subscriber(channel, "data", "webcam");
  
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