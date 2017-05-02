///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Renato Landim Vargas (118557)
//
///////////////////////////////////////////////////////////////////////////////
/*
//  _____ _____  _   _ _____   _____  _     _____ _____ _   _ _____ 
// |  ___/  __ \| | | |  _  | /  __ \| |   |_   _|  ___| \ | |_   _|
// | |__ | /  \/| |_| | | | | | /  \/| |     | | | |__ |  \| | | |
// |  __|| |    |  _  | | | | | |    | |     | | |  __|| . ` | | |
// | |___| \__/\| | | \ \_/ / | \__/\| |_____| |_| |___| |\  | | |
// \____/ \____/\_| |_/\___/   \____/\_____/\___/\____/\_| \_/ \_/
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
#include <arpa/inet.h>

#include "utils.hpp"

///////////////////////////////////////////////////////////////////////////////

#define SERVER_PORT 12345
#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////
// Creates a new socket
int socket() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(fd) + " created successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

///////////////////////////////////////////////////////////////////////////////
// Connects socket to address
void connect(int socket, struct hostent *address) {
  struct sockaddr_in socket_address;

  // Creates socket address structure
  bzero((char *)&socket_address, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(SERVER_PORT);
  bcopy((char *)address->h_addr, (char *)&socket_address.sin_addr.s_addr, address->h_length);

  // Connects to server
  int ret = connect(socket, (struct sockaddr*)&socket_address, sizeof(socket_address));

  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket) + " was connected");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

int main(int argc, char * argv[])
{
  struct hostent *host_address;
  char buf[MAX_LINE];
  std::string str_in, local_port, local_ip;

  // Checks given arguments
  if (argc < 2) {
    log::write(FAIL, "Missing hostname argument");
  }
  else if (argc > 2) {
    log::write(FAIL, "Too many arguments");
  }

  // Gets host by name from argument
  host_address = gethostbyname(argv[1]);

  // In case no host is found
  if (host_address == NULL) {
    log::write(FAIL, "No host found with given name");
    return EXIT_FAILURE;
  }

  // Creates active socket
  int server_fd = socket();

  // Connects socket to server
  connect(server_fd, host_address);

  // Gets local address
  struct sockaddr_in local_address;
  socklen_t local_len;
  getsockname(server_fd, (struct sockaddr *)&local_address, &local_len);
  local_ip = inet_ntoa(local_address.sin_addr);
  local_port = std::to_string(ntohs(local_address.sin_port));

  do {
    std::cout << "[" << local_ip << ":" << local_port << "] ";

    // Clears the string
    bzero(buf, sizeof(char)*MAX_LINE);

    // Reads console input and get string length
    std::getline(std::cin, str_in);
    str_in += '\n';
    int n = str_in.size();

    // Writes string to the server
    n = write(server_fd, str_in.c_str(), n);

    // In case it fails to write
    if (n <= 0) {
      log::write(FAIL, "Fail to write to server");
    }

    // Reads server echo
    n = read(server_fd, buf, MAX_LINE);

    // In case it fails to read
    if (n <= 0) {
      log::write(FAIL, "Fail to read from server");
    }

    // Shows server message
    std::cout << std::string(buf);
  } while(std::strcmp(str_in.c_str(), "exit\n"));

  // Closes connection
  close(server_fd);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
