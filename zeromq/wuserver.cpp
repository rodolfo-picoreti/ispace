#include <tuple>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>
#include <msgpack.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "zmq.hpp"

typedef std::tuple <int64_t, int, int, int, std::vector<unsigned char>> payload_t;

int main () {
  using namespace std::chrono;

  // Open camera device
  std::cout << "Starting camera... " << std::flush;
  cv::VideoCapture capture { 0 }; // open the default camera
  if (!capture.isOpened()) {
    std::cout << "Failed to open capture device" << std::endl;
  }
  std::cout << "[ok] " << std::endl;

  //  Prepare our context and publisher
  zmq::context_t context (1);
  zmq::socket_t publisher (context, ZMQ_PUB);
  publisher.bind("epgm://eth0;239.192.1.1:5555");
  //publisher.bind("ipc://webcam.ipc");               

  auto delta = milliseconds(static_cast<int>(std::round(1000.0/30.0)));
  while (1) {
    
    auto now = system_clock::now();

    cv::Mat frame;
    capture >> frame;

    std::vector<unsigned char> raw(frame.datastart, frame.dataend);
    payload_t payload {
      system_clock::now().time_since_epoch().count(),
      frame.rows, 
      frame.cols, 
      frame.type(), 
      raw
    }; 
    
    std::stringstream stream;
    msgpack::pack(stream, payload);
    std::string msg { stream.str() }; 
    
    zmq::message_t message(msg.begin(), msg.end());
    publisher.send(message);
    std::this_thread::sleep_until(now + delta);
  }

  return 0;
}