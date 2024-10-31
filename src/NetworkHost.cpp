#include "../include/fish_game/NetworkHost.hpp"

NetworkHost::NetworkHost() : stopThread(false) {
  workerThread = std::thread(&NetworkHost::threadFunction, this);
  std::cout << "Thread created \n";
}

NetworkHost::~NetworkHost() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stopThread = true;
  }
  cv.notify_all();
  workerThread.join();
}

std::optional<fish_game::InputEvent> NetworkHost::getActionQueue() {
  std::lock_guard<std::mutex> lock(mtx);
  if (elementQueue.empty()) {
    return std::nullopt;
  }
  fish_game::InputEvent element = elementQueue.front();
  elementQueue.pop();
  return element;
}

void NetworkHost::updateState(const std::string &updatedState) {
  std::lock_guard<std::mutex> lock(mtx);
  state = updatedState;
}

void NetworkHost::threadFunction() {
  SocketManager socketManager;

  while (true) {
    socketManager.run();
    std::cout << "Hello from thread \n";
    {
      std::unique_lock<std::mutex> lock(mtx);
      if (stopThread) {
        return;
      }
      // Simulate adding elements to the queue
      elementQueue.push(fish_game::InputEvent()); // Example element
    }
    cv.notify_one();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
  }
}
