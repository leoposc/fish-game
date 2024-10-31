
#ifndef NETWORK_HOST
#define NETWORK_HOST

#include "./GameInputEvents.hpp"
#include "./SocketManager.hpp"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class NetworkHost {
public:
  NetworkHost();
  ~NetworkHost();

  std::optional<fish_game::InputEvent> getActionQueue();
  void updateState(const std::string &updatedState);

private:
  void threadFunction();
  std::thread workerThread;
  std::mutex mtx;
  std::condition_variable cv;
  std::queue<fish_game::InputEvent> elementQueue;
  std::string state;
  bool stopThread;
};

#endif // NETWORK_HOST
