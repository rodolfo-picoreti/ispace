#ifndef __IS_SERVICE_HPP__
#define __IS_SERVICE_HPP__

#include <string>
#include <thread>
#include <atomic>
#include <unordered_map>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class Service {

  Channel::ptr_t channel;
  const std::string queue;
  const std::string exchange;

  typedef std::function<void (const Service&)> handle_t;
  std::unordered_map<std::string, handle_t> map;

  BasicMessage::ptr_t request;
  
  std::atomic_bool running;

public:

  Service(Channel::ptr_t channel, const std::string& service);
  
  void bind(const std::string& key, handle_t handle);
  void process();

  std::string body() const;
  void reply(const std::string& body) const;

  void stop();
  std::thread listen();
  
}; // ::Service

} // ::is

#endif // __IS_SERVICE_HPP__