#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "./GameInputEvents.hpp"
#include "./SocketManager.hpp"
#include <mutex>
#include <queue>
#include <string>
#include <thread>

class NetworkClient {
  public:
	NetworkClient(const std::string hostIP, const std::string username);
	~NetworkClient();

	void setEvent(const InputEvent::Event event);
	std::string getUpdate();

  private:
	// data variables
	std::string hostIP;
	std::string username;

	// functional variables
	SocketManager socket;
	std::thread workerThread;
	std::mutex mutex;
	std::queue<InputEvent::Event> sendQueue;

	void run();
};

#endif // NETWORK_CLIENT
