#include <tuple>
#include <string>
#include <iostream>
#include <chrono>
#include <mutex>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "../lib/publisher.hpp"
#include "../lib/broker.hpp"
#include "../lib/service.hpp"

typedef std::tuple <int, int, int, std::vector<unsigned char>> payload_t;

auto credentials = std::make_tuple("ispace", "ispace", "is");

void usage() {
  std::cout << "pub-image <service-name> \n"
            << "\t where <service-name> is the broker avahi service name" << std::endl; 
  exit(1);
}

std::mutex mutex;
auto delta = std::chrono::milliseconds(static_cast<int>(std::round(1000.0/30.0)));

void set_delta(double fps) {
  std::lock_guard<std::mutex> lock(mutex);
  delta = std::chrono::milliseconds(static_cast<int>(std::round(1000.0/fps)));
}

std::chrono::milliseconds get_delta() {
  std::lock_guard<std::mutex> lock(mutex);
  return delta;
}

void service_fps(is::Service& service) {
  std::tuple<double> args;
  service.args(args);
  
  double desired_fps = std::get<0>(args);
  std::string reply; 
  if (desired_fps > 0 && desired_fps <= 30) {
    set_delta(desired_fps);
    reply = "ok";
  } 
  else {
    reply = "bad args";
  }

  service.reply(std::make_tuple(reply));
}

int main(int argc, char const *argv[]) {
  if (argc != 2) usage();

  // Open camera device
  std::cout << "Starting camera... " << std::flush;
  cv::VideoCapture capture { 0 }; // open the default camera
  if (!capture.isOpened()) {
    std::cout << "Failed to open capture device" << std::endl;
  }
  std::cout << "[ok] " << std::endl;

  // Discover and connect to broker
  std::cout << "Searching for broker... " << std::flush;
  is::Broker broker;
  auto found = broker.discover(argv[1]);
  std::cout << "[ok]" << std::endl;
  
  // Add set fps service
  auto service_channel = broker.connect(found.front(), credentials);
  is::Service service(service_channel, "webcam");
  service.bind("webcam.fps", service_fps);
  service.listen();

  // Create data publisher
  auto pub_channel = broker.connect(found.front(), credentials);
  is::Publisher publisher(pub_channel, "data", "webcam");
  
  while (1) {
    static unsigned int count = 0;
    auto now = std::chrono::system_clock::now();

    cv::Mat frame;
    capture >> frame;

    std::vector<unsigned char> raw(frame.datastart, frame.dataend);
    payload_t payload {
      frame.rows, frame.cols, frame.type(), raw
    }; 

    publisher.publish(payload);

    std::cout << '\r' << count << std::flush;
    ++count;
    
    std::this_thread::sleep_until(now + get_delta());
  }

  return 0;
}