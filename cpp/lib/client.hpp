#ifndef __IS_CLIENT_HPP__
#define __IS_CLIENT_HPP__

#include <iostream>

#include <string>
#include <mutex>
#include <future>
#include <thread>
#include <sstream>
#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace is {

using namespace AmqpClient;

class Client {
  
  Channel::ptr_t channel;
  const std::string exchange;
  const std::string key;
  std::string queue;

  std::atomic_bool running;
  unsigned int correlationId;
  std::unordered_map<std::string, std::promise<msgpack::object>> map;
  std::thread listener;
/*
  void receive(); 
  void spawn(); // spawn background listener thread
  void make_request();
*/
public:

  Client(Channel::ptr_t channel, const std::string& key);
  ~Client();

/*
  template <typename Payload>
  std::future<msgpack::object> request(const Payload& payload) {
 
    // Create request
    std::stringstream body;
    msgpack::pack(body, payload);
    auto request = BasicMessage::Create(body.str());
    request->ReplyTo(queue);
    request->CorrelationId(std::to_string(correlationId)); 
    request->ContentType("application/msgpack");
    request->DeliveryMode(BasicMessage::dm_persistent);
    channel->BasicPublish(exchange, key, request);

    std::promise<msgpack::object> promise; 
    std::future<msgpack::object> future = promise.get_future();
  
    map.emplace(std::to_string(correlationId), std::move(promise));

    ++correlationId;
    return std::move(future);
  }
*/
};

} // ::is

#endif // __IS_CLIENT_HPP__