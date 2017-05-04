///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Ingrato
//
///////////////////////////////////////////////////////////////////////////////
/* 
//  _____ _____  _   _ _____   _____ ___________ _   _ ___________ 
// |  ___/  __ \| | | |  _  | /  ___|  ___| ___ \ | | |  ___| ___ \
// | |__ | /  \/| |_| | | | | \ `--.| |__ | |_/ / | | | |__ | |_/ /
// |  __|| |    |  _  | | | |  `--. \  __||    /| | | |  __||    / 
// | |___| \__/\| | | \ \_/ / /\__/ / |___| |\ \\ \_/ / |___| |\ \ 
// \____/ \____/\_| |_/\___/  \____/\____/\_| \_|\___/\____/\_| \_|
//  
*/
///////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <cassert>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include "utils.hpp"

///////////////////////////////////////////////////////////////////////////////

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////
// Creates a new socket
int socket() {
  // Creates socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  
  // Sets option to reuse socket 
  int optval = 1;
  int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if(fd >= 0 && opt >= 0) { // On success, print new file descriptor
    log::write(DEBUG, "Socket " + std::to_string(fd) + " created successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

///////////////////////////////////////////////////////////////////////////////
// Binds socket to address
void bind(int socket) {
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char *)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(LISTEN_PORT);

  int ret = bind(socket, (struct sockaddr*) &address, sizeof(address));

  if(ret >= 0) { // On success, log 
    log::write(DEBUG, "Socket " + std::to_string(socket) + " binded to port " + std::to_string(LISTEN_PORT) + " successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}

///////////////////////////////////////////////////////////////////////////////
//  Listen the socket for accepting connections
void listen(int socket) {
  int ret = listen(socket, MAX_PENDING);

  if(ret >= 0) { // On success, log
    log::write(DEBUG, "Socket " + std::to_string(socket) + " is listenning for new connections");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}

///////////////////////////////////////////////////////////////////////////////
// Waits for new connections
int accept(int socket, struct sockaddr_in& address) {
  socklen_t addrlen = sizeof(address);

  int fd = accept(socket, (struct sockaddr*) &address, &addrlen);

  if(fd >= 0) { // On success, log
    log::write(DEBUG, "New connection accepted: " + std::to_string(fd));
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  std::cout << "Client [" << inet_ntoa(address.sin_addr) << ":";
  std::cout << ntohs(address.sin_port) << "] connected!" << std::endl;
  
  return fd;
}

///////////////////////////////////////////////////////////////////////////////
// Receives data from client at a max of MAX_LINE bytes and puts it into the 
// buffer buf
int read(int fd, char* buff) {
  int n = read(fd, buff, MAX_LINE);
  
  if(n >= 0) { // On success, log
    log::write(DEBUG, "Received " + std::to_string(n) + " bytes");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  return n;
}

///////////////////////////////////////////////////////////////////////////////
// Handles the client. This function does the reads and writes and communicates 
// with the client. This function should run on a child proccess
bool client_handler(int client_fd, struct sockaddr_in address) {
  std::string ip = inet_ntoa(address.sin_addr);
  std::string port = std::to_string(ntohs(address.sin_port));
  char buf[MAX_LINE];

  // Clear the string
  bzero(buf, sizeof(char)*MAX_LINE);

  // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
  int n = read(client_fd, buf);

  if (n==0) return false; // Something bad happened to client

  // Shows client message on screen
  std::cout << "[" << ip << ":" << port << "]" << " says: " << std::string(buf) << std::endl;
  std::cout.flush();

  // Echoes back to client
  n = write(client_fd, buf, n);
  if (n >= 0) { // On success, log
    log::write(DEBUG, "Wrote " + std::to_string(n) + " bytes to client");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  // Client has exited
  if (!std::strcmp(buf, "exit\r\n") || !std::strcmp(buf, "exit\n") || !std::strcmp(buf, "exit")) {
    return false;
  }

  log::write(DEBUG, "Echoed to client [" + ip + ":" + port + "] successfully");
  return true;
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
  struct sockaddr_in address;
  int i, max_fd, n_clients, clients[FD_SETSIZE];
  fd_set all_fds, new_set;

  // Sets verbose level. If compiled in debug mode, show all messages, else
  // show only FAIL errors 
#ifdef NDEBUG
  log::level() = FAIL;
#else
  log::level() = DEBUG;
#endif

  // Creates socket and get a file descriptor of the socket
  int socket_fd = socket();

  // Binds the socket to the descriptor
  bind(socket_fd);

  //  Listen the socket for accepting connections
  listen(socket_fd);

  // Initializes variables
  n_clients = -1;
  for (i = 0; i < FD_SETSIZE; i++) {
    clients[i] = -1;
  }
  FD_ZERO(&all_fds);

  // Adds server socket to the set
  max_fd = socket_fd;
  FD_SET(socket_fd, &all_fds);

  // Server's main loop
  while (true) {
    // Creates copy of all_fds
    new_set = all_fds;

    // Checks how many clients have data
    int nready = select(max_fd + 1, &new_set, NULL, NULL, NULL);

    // Exits in case of error on select
    if (nready < 0) {
      log::write(FAIL, std::strerror(errno));
    }

    // Checks if new client is trying to connect
    if (FD_ISSET(socket_fd, &new_set)) {
      // Accepts new connection
      int client_fd = accept(socket_fd, address);

      // Saves file descriptor on vector
      for (i = 0; i < FD_SETSIZE; i++) {
        if (clients[i] < 0) {
          clients[i] = client_fd;
          break;
        }
      }

      // Disconnects client if max number of clients is reached
      if (i == FD_SETSIZE) {
        close(client_fd);
      }

      // Adds client file descriptor to the set
      FD_SET(client_fd, &all_fds);

      if (client_fd > max_fd)
        max_fd = client_fd; // Updates max file descriptor
      if (i > n_clients)
        n_clients = i; // Updates max index on vector
      if (--nready <= 0)
        continue; // There aren't and file descriptors to be read
    }

    // Verifies which clients have data to be read and proccess it
    for (i = 0; i <= n_clients; i++) {
      // Gets valid client file descriptor
      int client_fd = clients[i];
      if (client_fd < 0) continue;

      // Checks if client has data
      if (FD_ISSET(client_fd, &new_set)) {
        // Tries to read from client and echo message
        if (!client_handler(client_fd, address)) {
          // Closes connection if client has disconnected
          close(client_fd);
          FD_CLR(client_fd, &all_fds);
          clients[i] = -1;
          log::write(DEBUG, "Socket " + std::to_string(client_fd) + " has been disconnected");
        }

        // There aren't and file descriptors to be read
        if (--nready <= 0) break;
      }
    }
  }

  // Closes socket
  close(socket_fd);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
