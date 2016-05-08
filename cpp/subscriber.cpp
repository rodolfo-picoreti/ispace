#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#include <msgpack.hpp>

using namespace AmqpClient;

//g++ subscriber.cpp -std=c++11 `pkg-config --libs --cflags msgpack` `pkg-config --libs --cflags libSimpleAmqpClient`

int main() {
    
    const std::string host = "127.0.0.1";
    int port = 5672;
    const std::string username = "ispace";
    const std::string password = "ispace";
    const std::string vhost = "is";

    Channel::ptr_t channel;
    channel = Channel::Create(host, port, username, password, vhost); 

    std::string queue_name = channel->DeclareQueue("");

    const std::string exchange_name = "1";
    const std::string routing_key = "*.info";
    channel->BindQueue(queue_name, exchange_name, routing_key);

    channel->BasicConsume(queue_name, "hodor");

    while (1) {
        BasicMessage::ptr_t msg_out = channel->BasicConsumeMessage("hodor")->Message(); 
        auto body = msg_out->Body();
        std::size_t offset = 0;

        auto unpacked = msgpack::unpack(body.data(), body.size(), offset);
        auto obj = unpacked.get();

        std::cout << obj << std::endl;
    }

}

