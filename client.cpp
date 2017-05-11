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

#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////
// Creates a new socket
int socket() {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  // Sets option to reuse socket
  int optval = 1;
  int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (fd >= 0 && opt >= 0) { // On success, print new file descriptor
    log::write(DEBUG, "Socket " + std::to_string(fd) + " created successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
  struct hostent *host_address;
  char buf[MAX_LINE];
  std::string str_in, local_port, local_ip;
  int server_port;

  // Sets verbose level. If compiled in debug mode, show all messages, else
  // show only FAIL errors
#ifdef NDEBUG
  log::level() = FAIL;
#else
  log::level() = DEBUG;
#endif

  // Checks given arguments
  if (argc != 3) {
    log::write(FAIL, "Usage: server hostname port");
  }

  // Gets port and host by name from argument
  host_address = gethostbyname(argv[1]);
  server_port = std::stoi(argv[2]);

  // In case no host is found
  if (host_address == NULL) {
    log::write(FAIL, "No host found with given name");
  }

  // Creates active socket
  int server_fd = socket();

  struct sockaddr_in local_address, server_address;
  socklen_t local_len;
  
  // Gets local address
  bzero((char *)&local_address, sizeof(local_address));
  getsockname(server_fd, (struct sockaddr *)&local_address, &local_len);
  local_ip = inet_ntoa(local_address.sin_addr);
  local_port = std::to_string(ntohs(local_address.sin_port));

  // Gets server address
  bzero((char *)&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(server_port);
  bcopy((char *)host_address->h_addr, (char *)&server_address.sin_addr.s_addr, host_address->h_length);

  while(true) 
  {
    std::cout << "[" << local_ip << ":" << local_port << "] ";

    // Clears the string
    bzero(buf, sizeof(char)*MAX_LINE);

    // Reads console input
    std::getline(std::cin, str_in);
    // Truncates if string is too big
    if (str_in.size() >= MAX_LINE) {
      str_in.resize(MAX_LINE - 1);
    }
    // Adds newline and gets string length
    str_in += '\n';
    if(!str_in.compare("exit\n")) break;

    // Writes string to the server
    int n = sendto(server_fd, str_in.c_str(), str_in.size(), 0, (const struct sockaddr*)&server_address, sizeof(server_address));
    
    // In case it fails to write
    if (n <= 0) {
      log::write(FAIL, strerror(errno));
    }

    // Reads server echo
    n = recvfrom(server_fd, buf, MAX_LINE, 0, NULL, NULL);

    // In case it fails to read
    if (n <= 0) {
      log::write(FAIL, "Fail to receive from server");
    }

    // Shows server message
    std::cout << std::string(buf);
  }

  // Closes connection
  close(server_fd);

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
