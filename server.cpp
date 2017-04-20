///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Renato Landim Vargas (118557)
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
#include <sys/types.h>
#include <sys/socket.h>

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
int accept(int socket) {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  int fd = accept(socket, (struct sockaddr*) &address, &addrlen);

  if(fd >= 0) { // On success, log
    log::write(DEBUG, "New connection accepted: " + std::to_string(fd));
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

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

int main()
{
  char buf[MAX_LINE];

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

  // Server's main loop
  while(true) {
    // Accepts new connections
    int client_fd = accept(socket_fd);

    do {
      // Clear the string
      bzero(buf, sizeof(char)*MAX_LINE);

      // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
      int n = read(client_fd, buf);

      if(n==0) break; // Something bad happened to client

      // Shows client message on screen
      std::cout << "Client says: " << std::string(buf);
      std::cout.flush();

      // Echoes back to client
      n = write(client_fd, buf, n);
      if(n >= 0) { // On success, log
        log::write(DEBUG, "Wrote " + std::to_string(n) + " bytes to client");
      }
      else { // On fail, print error
        log::write(FAIL, std::strerror(errno));
      }
    } while(std::strcmp(buf, "exit\r\n") && std::strcmp(buf, "exit\n"));

    // Closes connection
    close(client_fd);
  }

  // Closes socket
  close(socket_fd);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
