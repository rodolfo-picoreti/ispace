#include <iostream>
#include <string>
#include <chrono>

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "../lib/service.hpp"

const std::string host { "127.0.0.1" };
int port { 5672 };
const std::string username { "ispace" };
const std::string password { "ispace" };
const std::string vhost { "is" };

void echo(const is::Service& service) {
  std::string body = service.body();
  std::cout << " [x] Received: " << body << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  service.reply(body);    
}

int main() {    
  auto channel = AmqpClient::Channel::Create(host, port, username, password, vhost); 
  
  is::Service service(channel, "echo");
  service.bind("echo.exec", echo);

  auto bg = service.listen();
  bg.join();
}
