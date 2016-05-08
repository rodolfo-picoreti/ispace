#include <iostream>
#include <stdlib.h>
#include <string>
#include <tuple>

#include <msgpack.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main() {
    using namespace AmqpClient;
    
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

    channel->BasicConsume(queue_name);

    while (1) {
        BasicMessage::ptr_t message;
        message = channel->BasicConsumeMessage()->Message(); 
        
        auto body = message->Body();
        
        if (message->ContentType() == "application/msgpack") {
            std::size_t offset = 0;

            auto unpacked = msgpack::unpack(body.data(), body.size(), offset);
            auto payload = unpacked.get();

            std::cout << " [x] Received: " << payload << "\n";
        } 
        else if (message->ContentType() == "application/json") {
            std::cout << " [x] Received: " << body << "\n";
        }
        else {
            std::cout << " [!] Invalid ContentType: " << message->ContentType() << "\n";
        }
    }
}

