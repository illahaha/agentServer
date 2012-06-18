#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

class CNetModule
{
 public:
  CNetModule();
 public:
  void startNetService();
  void stopNetService();
 private:
  int setnonblocking(int sockfd);
  int handle_message(int new_fd);
};
