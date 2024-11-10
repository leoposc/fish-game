#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <arpa/inet.h>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define BUFFER_SIZE 1024

struct IncomingMessage {
	int client_id;
	std::string message;
};

class SocketManager {
  public:
	SocketManager(int port, bool host);
	~SocketManager();
	void sendMessage(std::string message);
	IncomingMessage popMessage();

  private:
	void setupServer(int port);
	void setupClient(int port);
	void run(int client_socket);

	int server_fd, socket_id;
	struct sockaddr_in address;
	int addrlen;
	std::vector<int> client_sockets;
	std::vector<std::thread> client_threads;
	std::mutex mtx;
	std::condition_variable cv;
	bool stopThread = false;
	std::thread server_thread;
	std::vector<IncomingMessage> messages;
};

#endif // SOCKET_MANAGER
