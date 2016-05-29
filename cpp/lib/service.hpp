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
  std::thread listener;

public:

  Service(Channel::ptr_t channel, const std::string& service);
  ~Service();

  void bind(const std::string& key, handle_t handle);
  void process();

  void stop();
  void listen();
  void listen_sync();

public:

  template <typename Payload>
  void args(Payload& payload) {
    std::string body(request->Body());
    msgpack::object_handle handle = msgpack::unpack(body.data(), body.size());
    
    // object is valid during the object_handle instance is alive.
    msgpack::object object = handle.get();
    object.convert(payload);
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