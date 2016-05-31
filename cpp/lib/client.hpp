#ifndef __IS_CLIENT_HPP__
#define __IS_CLIENT_HPP__

#include <iostream>

#include <string>
#include <sstream>
#include <chrono>
#include <set>
#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <algorithm>

namespace is {

using namespace AmqpClient;

typedef unsigned int uint;

class Client {
  
  Channel::ptr_t channel;
  const std::string exchange;
  const std::string key;
  std::string queue;

  uint correlationId;
  std::unordered_map<uint, BasicMessage::ptr_t> map;

public:

  Client(Channel::ptr_t channel, const std::string& key);

  bool receive(const uint& id, const uint& timeout);
  bool receive(const std::set<uint>& ids, const uint& timeout);

  template <typename Payload>
  bool consume(const uint& id, Payload& payload) {
    auto iterator = map.find(id);
    if (iterator == map.end()) return false;

    std::string body(iterator->second->Body());
    msgpack::object_handle handle = msgpack::unpack(body.data(), body.size());
    
    // object is valid during the object_handle instance is alive.
    msgpack::object object = handle.get();
    object.convert(payload);

    map.erase(iterator); // delete message
    return true;
  }

  template <typename Payload>
  uint request(const Payload& payload) {  
    uint id = correlationId;
    ++correlationId;

    std::stringstream body;
    msgpack::pack(body, payload);
    auto request = BasicMessage::Create(body.str());
    request->ReplyTo(queue);
    request->CorrelationId(std::to_string(id)); 
    request->ContentType("application/msgpack");
    request->DeliveryMode(BasicMessage::dm_persistent);
    channel->BasicPublish(exchange, key, request);
    
    return id;
  }

};

} // ::is

#endif // __IS_CLIENT_HPP__