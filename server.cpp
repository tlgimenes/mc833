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
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
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
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  
  // Sets option to reuse socket 
  int optval = 1;
  int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if(fd >= 0 && opt >= 0) { // On success, print new file descriptor
    log::write(DEBUG, "UDP socket " + std::to_string(fd) + " created successfully");
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

int main()
{
  struct sockaddr_in cli_addr; // client address
  char buf[MAX_LINE];          // received buffer
  socklen_t cli_len;           // client address length

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

  // Server's main loop
  while(true) {
    // Clear the string
    bzero(buf, sizeof(char)*MAX_LINE);

    // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
    ssize_t n = recvfrom(socket_fd, buf, sizeof(char)*MAX_LINE, 0, (struct sockaddr*)&cli_addr, &cli_len);
    // If didn't receive anything or if something bad happend to client
    if(n==0) continue;

    // Clients ip and port
    std::string ip = inet_ntoa(cli_addr.sin_addr);
    std::string port = std::to_string(ntohs(cli_addr.sin_port));

    // Shows client message on screen
    std::cout << "[" << ip << ":" << port << "]" << " says: " << std::string(buf) << std::endl;
    std::cout.flush();

    // Echoes back to client
    n = sendto(socket_fd, buf, sizeof(char)*n, 0, (const struct sockaddr *)&cli_addr, cli_len);
    if(n >= 0) { // On success, log
      log::write(DEBUG, "Wrote " + std::to_string(n) + " bytes to client");
    }
    else { // On fail, print error
      log::write(FAIL, std::strerror(errno));
    }
  }

  // Closes socket
  close(socket_fd);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
