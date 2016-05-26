#ifndef __IS_SERVICE_HPP__
#define __IS_SERVICE_HPP__

#include <iostream>

#include <string>
#include <thread>
#include <atomic>
#include <unordered_map>

#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class Service {

  Channel::ptr_t channel;
  const std::string queue;
  const std::string exchange;

  typedef std::function<void (Service&)> handle_t;
  std::unordered_map<std::string, handle_t> map;

  BasicMessage::ptr_t request;
  std::atomic_bool running;

public:

  Service(Channel::ptr_t channel, const std::string& service);
  
  void bind(const std::string& key, handle_t handle);
  void process();

  void stop();
  std::thread listen();

public:

  template <typename Payload>
  void args(Payload& payload) {
    auto body = request->Body();
    std::size_t offset = 0;
    auto unpacked = msgpack::unpack(body.data(), body.size(), offset).get();
    unpacked.convert(payload);
  }

  template <typename Payload>
  void reply(const Payload& payload) const {
    std::stringstream body;
    msgpack::pack(body, payload);
    auto response = BasicMessage::Create(body.str());
    response->ContentType("application/msgpack");
    response->CorrelationId(request->CorrelationId());
    channel->BasicPublish(exchange, request->ReplyTo(), response);
  }
  
  
}; // ::Service

} // ::is

#endif // __IS_SERVICE_HPP__