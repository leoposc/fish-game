#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "./GameInputEvents.hpp"
#include <string>

class NetworkClient {
public:
  NetworkClient(const std::string &hostIP, const std::string &username);
  ~NetworkClient();

  void setEvent(const fish_game::InputEvent &event);
  std::string getUpdate();

private:
  std::string hostIP;
  std::string username;
};

#endif // NETWORK_CLIENT
