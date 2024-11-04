#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <arpa/inet.h>
#include <condition_variable>
#include <cstring> // Add this include at the top of your file
#include <cstring>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define BUFFER_SIZE 1024

class SocketManager {
public:
  SocketManager(int port, bool host);
  ~SocketManager();
  std::string popMessage();
  void sendMessage(std::string);

private:
  void run();
  void setupServer(int port);
  void setupClient(int port);
  std::thread workerThread;
  std::mutex mtx;
  bool stopThread;
  std::condition_variable cv;
  int server_fd;
  int socket_id;
  struct sockaddr_in address;
  std::vector<std::string> messages;
  int port;
  int addrlen;
};

#endif // SOCKET_MANAGER
