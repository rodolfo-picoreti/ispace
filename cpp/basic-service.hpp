#ifndef __IS_BASIC_SERVICE_HPP__
#define __IS_BASIC_SERVICE_HPP__

#include <iostream>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <vector>
#include <thread>
#include <unordered_map>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class BasicService {

  Channel::ptr_t channel;
  const std::string queue;
  const std::string exchange;

  typedef std::function<void (const BasicService&)> handle_t;
  std::unordered_map<std::string, handle_t> map;

  BasicMessage::ptr_t request;
  
public:

  BasicService(Channel::ptr_t chann, const std::string& service);
  
  void bind(const std::string& key, handle_t handle);
  void process();

  std::string body() const;
  void reply(const std::string& body) const;

}; // ::BasicService

} // ::is

#endif // __IS_BASIC_SERVICE_HPP__