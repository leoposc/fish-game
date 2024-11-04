#include "../include/fish_game/NetworkHost.hpp"

NetworkHost::NetworkHost() : stopThread(false) {
  this->workerThread = std::thread(&NetworkHost::threadFunction, this);
  std::cout << "Thread created \n";
}

NetworkHost::~NetworkHost() {
  std::cout << "NetworkHost deconstructed \n";
  this->stopThread = true;
  this->cv.notify_all();
  this->workerThread.join();
  std::cout << "SocketManager deconstructed finished\n";
}

std::optional<fish_game::InputEvent> NetworkHost::getActionQueue() {
  std::lock_guard<std::mutex> lock(mtx);
  if (this->elementQueue.empty()) {
    return std::nullopt;
  }
  fish_game::InputEvent element = this->elementQueue.front();
  this->elementQueue.pop();
  return element;
}

void NetworkHost::updateState(const std::string &updatedState) {
  std::lock_guard<std::mutex> lock(mtx);
  this->state = updatedState;
}

void NetworkHost::threadFunction() {
  this->registerSocket = std::make_unique<SocketManager>(8080, true);
  int counter = 0;
  while (true) {
    counter++;
    std::cout << "Hello from thread \n";
    {
      std::lock_guard<std::mutex> lock(mtx);
      if (stopThread) {
        return;
      }
      // Simulate adding elements to the queue
      elementQueue.push(fish_game::InputEvent()); // Example element
    }
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
  }
}
