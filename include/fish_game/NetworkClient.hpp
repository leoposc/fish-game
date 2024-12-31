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

	// what should the network host have?
	// Needs all players, so it should also have all the elements -> first time transfer of everything?
	// How do the other clients get all their objects?
	// Should the network client on register send all of its objects? Or should the server create all the components?
	// If the server creates all the components then first time they should be transferred fully the first time

	// PROTOCOL:
	// 1. Server created
	// 2. Own clients joins -> server creates player, broadcasts player to all clients (inlcuding just joined player)
	// 3. New client joins -> server creates new player, broadcasts again
	// 4. client1 sends button Press -> server receives, updates its player broadcasts the delta out
	//
	// Where do we call all the updateObjects? In the loop would probably be best?
	//
	//
	// host:    - needs to monitor what fields change to create deltas
	//          - processes all events, own and incoming
	//
	// clinet:  - needs to update his own stuff from deltas
	//          - needs to disable generation of items etc
	//          - sends input to host and processes them

	// functional variables
	SocketManager socket;
	std::thread workerThread;
	std::mutex mutex;
	std::queue<std::string> sendQueue;

	std::vector<std::tuple<std::string, int>> newUsers;
	std::string gameState;
	bool hasUpdateVal = false;

	void handleReceive();

	void run();
};

#endif // NETWORK_CLIENT
