#include "service-provider.hpp"

namespace is {

using namespace std;
using namespace AmqpClient;

ServiceProvider::ServiceProvider(Channel channel, const string& service, const string& exchange) 
  : channel(channel), service(service), exchange(exchange), running(false) {    

  // passive durable exclusive auto_delete 
  channel->DeclareQueue(service, false, true, false, false);
  
  // passive durable auto_delete 
  auto xtype = AmqpClient::Channel::EXCHANGE_TYPE_TOPIC;
  channel->DeclareExchange(exchange, xtype, false, true, false);
  channel->BindQueue(service, exchange, service + ".*");
  
  // consumer_tag no_local no_ack exclusive
  channel->BasicConsume(service, "", true, true, false);
}

ServiceProvider::~ServiceProvider() {
  if (running.load()) {
    stop(); // stop service provider thread
  }
}

void ServiceProvider::expose(const string& name, handle_t handle) {
  // Assert the service thread is not running otherwise we would have a race condition
  assert(running.load() == false);
  map.emplace(service + '.' + name, handle);
}

void ServiceProvider::process() {

  Envelope::ptr_t envelope;
  
  while (1) {
    unsigned int timeout = 10; // milliseconds;
    bool newmsg;
  
    do {
      newmsg = channel->BasicConsumeMessage(envelope, timeout);
      if (running.load() == false) {
        return; // stop was called, leave!
      }
    } while (newmsg == false);

    message = envelope->Message();  
    
    if (message->ContentType() == "application/msgpack" &&
        message->CorrelationIdIsSet() == true           &&
        message->ReplyToIsSet() == true) {
      break;
    } 
    else {
      // Invalid message
    }
  }
  
  auto key = envelope->RoutingKey();
  auto handle = map.find(key);
  if (handle != map.end()) {
    handle->second(*this);
  } 
}

void ServiceProvider::stop() {
  running.store(false);
  listener.join(); // and wait until it finishes
}

void ServiceProvider::listen() {
  running.store(true);

  listener = thread([&]() {
    while (running.load()) {
      this->process();
    }
    return 0;
  });
}

void ServiceProvider::listen_sync() {
  while (1) {
    this->process();  
  }
}

} // ::is