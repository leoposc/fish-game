
#ifndef NETWORK_HOST
#define NETWORK_HOST

#include "./GameInputEvents.hpp"
#include "./SocketManager.hpp"
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

class NetworkHost {
  public:
	NetworkHost();
	~NetworkHost();

	std::optional<InputEvent::Event> getAction();
	void updateState(const std::string &updatedState);

	std::vector<std::string> getClients();

  private:
	void threadFunction();

	// data variables
	std::string state;
	std::queue<std::tuple<std::string, InputEvent::Event>> elementQueue;
	std::map<int, std::string> clients;

	// functionality variables
	std::condition_variable cv;
	std::thread workerThread;
	std::mutex mtx;
	bool stopThread;
	SocketManager socket;
};

#endif // NETWORK_HOST
