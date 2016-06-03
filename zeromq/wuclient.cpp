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

unsigned int latency(const int64_t& timestamp) {
  using namespace std::chrono;

  auto now = system_clock::now().time_since_epoch().count();
  auto diff = nanoseconds(now - timestamp);
  return duration_cast<milliseconds>(diff).count();
}

int main (int argc, char *argv[]) {
  zmq::context_t context (1);
  zmq::socket_t subscriber (context, ZMQ_SUB);
  //subscriber.connect("tcp://localhost:5556");
  //subscriber.connect("ipc://webcam.ipc");               
  subscriber.connect("epgm://eth0;239.192.1.1:5555");               

  const char *filter = "";
  subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

  while (1) {
    payload_t payload;
    zmq::message_t update;
    subscriber.recv(&update);
      
    std::string msg(static_cast<char*>(update.data()), update.size());  
    msgpack::object_handle handle = msgpack::unpack(msg.data(), msg.size());
    msgpack::object object = handle.get();
    object.convert(payload);

    int64_t timestamp = std::get<0>(payload);
    int rows = std::get<1>(payload);
    int cols = std::get<2>(payload);
    int type = std::get<3>(payload);
    unsigned char* data = std::get<4>(payload).data();
    int size = std::get<4>(payload).size();

    std::cout << "\r  \r" << latency(timestamp) << std::flush;
    
    cv::Mat image(rows, cols, type, data, size/rows);
    cv::imshow("Webcam stream", image);
    cv::waitKey(1);
  }  
  return 0;
}