#ifndef __IS_BROKER_HPP__
#define __IS_BROKER_HPP__

#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include <tuple>
#include <string>

#include "avahi.hpp"

namespace is {

using namespace AmqpClient;

typedef std::tuple<std::string, std::string, std::string> Credentials; 

class Broker {

  is::Avahi avahi;

public:

  Broker();

  const std::vector<Avahi::Service>& discover(const std::string& hostname, int port = 5672); 
  Channel::ptr_t connect(const Avahi::Service& service, const Credentials& credentials);

};

} // ::is

#endif // __IS_BROKER_HPP__