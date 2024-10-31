#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

class SocketManager {
public:
  SocketManager();
  ~SocketManager();
  void run();

private:
  int server_fd;
  struct sockaddr_in address;
  int addrlen;
};

#endif // SOCKET_MANAGER
