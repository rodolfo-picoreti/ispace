#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include <algorithm>
#include <iterator>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "../lib/publisher.hpp"
#include "../lib/subscriber.hpp"

const std::string host { "127.0.0.1" };
int port { 5672 };
const std::string username { "ispace" };
const std::string password { "ispace" };
const std::string vhost { "is" };

const std::string path { "/home/melchior/Desktop/len_full.jpg" };

int main(int argc, char const *argv[]) {
  
  auto channel = AmqpClient::Channel::Create(host, port, username, password, vhost); 
  is::Publisher publisher(channel, "data", "lena");
  is::Subscriber subscriber(channel, "data", "lena");
  
  {
    cv::Mat image = cv::imread(path.c_str());
    std::vector<unsigned char> imageRaw(image.datastart, image.dataend);
    std::tuple<int, int, int, std::vector<unsigned char>> payload {
      image.rows, image.cols, image.type(), imageRaw
    }; 
    publisher.publish(payload);
  }

  {
    auto received = subscriber.consume().get();
    std::cout << received << std::endl;
    std::tuple<int, int, int, std::vector<unsigned char>> payload;
    received.convert(payload);

    int rows = std::get<0>(payload);
    int cols = std::get<1>(payload);
    int type = std::get<2>(payload);
    unsigned char* data = std::get<3>(payload).data();
    cv::Mat image(rows, cols, type, data);
    cv::imshow("Received image", image);
    while (cv::waitKey(0) != 27);
  }

  return 0;
}