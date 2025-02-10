#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "./ECS/ECS.hpp"
#include "./GameInputEvents.hpp"
#include "./SocketManager.hpp"
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#define JOIN_PREFIX "JOIN:"
#define UPDATE_PREFIX "UPDATE:"

class NetworkClient {
  public:
	NetworkClient();
	~NetworkClient();

	void init(const std::string hostIP, const std::string username);

	void sendEvent(const std::string event);
	std::string getUpdate();
	bool hasUpdate();

  private:
	// data variables
	std::string hostIP;
	std::string username;
	int player_id;

	// functional variables
	SocketManager socket;
	std::thread workerThread;
	std::mutex mutex;
	std::queue<std::string> sendQueue;

	std::vector<std::tuple<std::string, int>> newUsers;
	std::string gameState;
	bool hasUpdateVal = false;
	bool isJoined = false;
	bool isRunning = true;

	void handleReceive();
	void run();
};

#endif // NETWORK_CLIENT
