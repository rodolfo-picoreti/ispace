#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
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
    const std::string exchange_type = Channel::EXCHANGE_TYPE_TOPIC;
    const std::string routing_key = "app.info";

    channel->DeclareExchange(exchange_name, exchange_type);

    auto payload = std::make_tuple(true, "ABC", 42);
    std::stringstream body;
    msgpack::pack(body, payload);

    auto message = BasicMessage::Create(body.str());
    message->ContentType("application/msgpack");

    while (1) {
        channel->BasicPublish(exchange_name, routing_key, message);
        std::cout << " [x] Sent : " << body.str() << " Size: " << body.str().length() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

