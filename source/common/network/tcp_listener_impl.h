#pragma once

#include "envoy/common/random_generator.h"
#include "envoy/runtime/runtime.h"

#include "absl/strings/string_view.h"
#include "base_listener_impl.h"

namespace Envoy {
namespace Network {

/* toa socket options, now only for nat64 */
enum {
    TOA_BASE_CTL            = 4096,
    TOA_SO_SET_MAX          = TOA_BASE_CTL,
    TOA_SO_GET_LOOKUP       = TOA_BASE_CTL,
    TOA_SO_GET_MAX          = TOA_SO_GET_LOOKUP,
};

struct toa_nat64_peer {
    struct in6_addr saddr;
    uint16_t sport;
};

union sockaddr_types {
    struct sockaddr_storage storage;
    struct sockaddr addr; 
    struct sockaddr_in in4;
    struct sockaddr_in6 in6;
};

/**
 * libevent implementation of Network::Listener for TCP.
 */
class TcpListenerImpl : public BaseListenerImpl {
public:
  TcpListenerImpl(Event::DispatcherImpl& dispatcher, Random::RandomGenerator& random,
                  SocketSharedPtr socket, TcpListenerCallbacks& cb, bool bind_to_port,
                  uint32_t backlog_size);
  ~TcpListenerImpl() override { socket_->ioHandle().resetFileEvents(); }
  void disable() override;
  void enable() override;
  void setRejectFraction(float reject_fraction) override;

  static const absl::string_view GlobalMaxCxRuntimeKey;

protected:
  void setupServerSocket(Event::DispatcherImpl& dispatcher, Socket& socket);

  TcpListenerCallbacks& cb_;
  const uint32_t backlog_size_;

private:
  void onSocketEvent(short flags);

  // Returns true if global connection limit has been reached and the accepted socket should be
  // rejected/closed. If the accepted socket is to be admitted, false is returned.
  static bool rejectCxOverGlobalLimit();

  Random::RandomGenerator& random_;
  float reject_fraction_;
};

} // namespace Network
} // namespace Envoy
