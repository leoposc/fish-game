#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "./GameInputEvents.hpp"
#include "./SocketManager.hpp"
#include <concepts>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

template <typename T>
concept HasFoo = requires(T t) {
	{ t.foo() } -> std::same_as<void>;
};

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

	// generate id local to the network client
	// generate ids for templates that accept objects
	// combine ids to send to server

	void run();
};

#endif // NETWORK_CLIENT
