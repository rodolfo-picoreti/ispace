#include <string>
#include <chrono>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include "image.hpp"
#include "subscriber.hpp"
#include "publisher.hpp"

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

void usage() {
  cout << "main <pub/sub> <broker-hostname> <topic>" << endl; 
  exit(1);
}

void publish(auto info, auto channel, auto topic) {
  auto capture = cv::VideoCapture(0); // open the default camera
  assert(capture.isOpened());

  auto publisher = is::Publisher(channel, "webcam");

  while (1) {
    auto now = system_clock::now();
    cv::Mat frame;
    capture >> frame;
    publisher.publish(to_image(frame), topic); 
    this_thread::sleep_until(now + 33ms);
  }
}

void subscribe(auto info, auto channel, auto topic) {
  auto subscriber = is::Subscriber(channel, "webcam." + topic);

  while (1) {
    Image image;
    subscriber.consume(image);   
    cout << "\r   \r" << subscriber.latency() << std::flush;
    cv::imshow("Webcam stream", to_mat(image));
    cv::waitKey(1); 
  }
}

int main(int argc, char const *argv[]) {
  if (argc != 4) usage();

  string mode = argv[1];
  string hostname = argv[2];
  string topic = argv[3];

  auto info = is::discover(hostname);
  auto channel = is::connect(info, { "ispace", "ispace", "/" });
  
  if (mode == "pub") publish(info, channel, topic);
  if (mode == "sub") subscribe(info, channel, topic);
}