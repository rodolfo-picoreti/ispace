#ifndef __IS_DISCOVERY_HPP__
#define __IS_DISCOVERY_HPP__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

#include <iostream>
#include <exception>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <vector>
#include <functional>

namespace is {

class Avahi {
  AvahiClient* client;
  AvahiServiceBrowser* browser;
  AvahiSimplePoll* simple_poll;
  bool outdated;
  unsigned int queued_resolvers;

public:

  struct Service {
    Service(const char* name, const char* domain, const char* host, 
      const char* type, char* ip, uint16_t port) 
      : name(name), domain(domain), host(host), type(type), ip(ip), port(port) {}

    std::string name;
    std::string domain;
    std::string host;
    std::string type;
    std::string ip;
    uint16_t port;
  };

private:

  typedef std::function<bool (const Service&)> filter_t;
  filter_t filter;
  std::vector<Service> services;

public:

  Avahi(const char* type, filter_t filter = [] (const Service&) { return true; });
  ~Avahi();

  void discover();
  const std::vector<Service>& discovered();

private:

  static void client_callback(AvahiClient* client, AvahiClientState state, void* data);

  static void browse_callback(
    AvahiServiceBrowser* browser, AvahiIfIndex interface, 
    AvahiProtocol protocol, AvahiBrowserEvent event,
    const char* name, const char* type, const char* domain, 
    AvahiLookupResultFlags flags, void* data);

  static void resolve_callback(
    AvahiServiceResolver* resolver, AvahiIfIndex interface,
    AvahiProtocol protocol, AvahiResolverEvent event,
    const char* name, const char* type, const char* domain, const char* host,
    const AvahiAddress* address, uint16_t port, 
    AvahiStringList* txt, AvahiLookupResultFlags flags, void* data);

};

} // ::is

#endif // __IS_DISCOVERY_HPP__