#include <chrono>
#include <vector>
#include <iostream>
#include <msgpack.hpp>

#include "service-provider.hpp"
#include "service-client.hpp"

struct HelloRequestType {
  std::string name;
  MSGPACK_DEFINE(name);
};

struct HelloReplyType {
  std::string message;
  MSGPACK_DEFINE(message);
};

void hello(is::ServiceProvider& service) {
  HelloRequestType request;
  service.request(request);

  HelloRequestType reply {
    "Hello " + request.name
  };
  service.reply(reply);
}

int main() {
  auto info = is::discover("melchior");
  auto server_channel = is::connect(info, { "ispace", "ispace", "/" });
  auto client_channel = is::connect(info, { "ispace", "ispace", "/" });

  // Service Provider  
  is::ServiceProvider server(server_channel, "ptgrey0");
  server.expose("hello", hello);
  server.listen();

  // Service Client  
  is::ServiceClient client(client_channel);
  is::GroupRequest group(client);
  
  HelloRequestType request { "Hodor" };
  auto id1 = client.request("ptgrey0.hello", request);
  auto id2 = client.request("ptgrey0.hello", request);
  auto ids = client.receive({id1, id2}, 1000);

  for (auto& id : ids) {
    HelloReplyType reply;
    client.consume(id, reply);
    std::cout << "id: " << id << ", received: " << reply.message << std::endl;
  }

  return 0;
} 