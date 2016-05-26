#ifndef __IS_BROKER_HPP__
#define __IS_BROKER_HPP__

#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "avahi.hpp"

namespace is {

using namespace AmqpClient;

class Broker {
  
  Channel::ptr_t _channel;
  const std::string hostname;
  int port { 5672 };
  const std::string username { "ispace" };
  const std::string password { "ispace" };
  const std::string vhost { "is" };

public:
  Broker(
    const std::string& hostname = "rabbit", int port = 5672,
    const std::string& username = "ispace", 
    const std::string& password = "ispace",
    const std::string& vhost = "is");

  void discover(); 

  Channel::ptr_t channel();

};

} // ::is

#endif // __IS_BROKER_HPP__