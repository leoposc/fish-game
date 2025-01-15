
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

#define JOIN_PREFIX "JOIN:"
#define UPDATE_PREFIX "UPDATE:"

typedef void (*FuncPtr)();

class NetworkHost {
  public:
	NetworkHost();
	~NetworkHost();

	std::optional<std::string> getAction();
	void updateState(const std::string &updatedState);

	std::vector<std::string> getClients();

	void printEventQueue();

  private:
	void notifyJoin(std::string username, int client_id);

	void threadFunction();

	// data variables
	std::string state;
	std::queue<std::tuple<std::string, std::string>>
	    elementQueue; // first element is the username/id second the actual event

	// the int is the id, it comes from the socket id
	std::map<int, std::string> clients;

	// functionality variables
	std::condition_variable cv;
	std::thread workerThread;
	std::mutex mtx;
	bool stopThread;
	SocketManager socket;

	// TODO: - send that player joined the game
	//       - method to send new state to all clients (string) (Semi done with updateState)
	//       - define prefixes to define what function was send: "JOINED:" for join followed by object string
};

#endif // NETWORK_HOST
