///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Renato Landim Vargas (118557)
//
///////////////////////////////////////////////////////////////////////////////
//
//  _____ _____  _   _ _____   _____  _     _____ _____ _   _ _____ 
//  |  ___/  __ \| | | |  _  | /  __ \| |   |_   _|  ___| \ | |_   _|
//  | |__ | /  \/| |_| | | | | | /  \/| |     | | | |__ |  \| | | |  
//  |  __|| |    |  _  | | | | | |    | |     | | |  __|| . ` | | |  
//  | |___| \__/\| | | \ \_/ / | \__/\| |_____| |_| |___| |\  | | |  
//  \____/ \____/\_| |_/\___/   \____/\_____/\___/\____/\_| \_/ \_/  
//                                                                   
//
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

#define SERVER_PORT 12345
#define MAX_LINE 256

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
  struct hostent *host_address;
  struct sockaddr_in socket_address;
  char *host;
  char buf[MAX_LINE];
  std::string str_in;
  int server_fd;
  int len;

  // Checks given arguments
  if (argc < 2) {
      log::write(FAIL, "Missing hostname argument");
  }
  else if (argc > 2) {
      log::write(FAIL, "Too many arguments");
  }

  // Gets address from host name
  host = argv[1]; 
  host_address = gethostbyname(host);

  if (host_address == NULL) {
      log::write(FAIL, "No host found with given name");
  }

  // Creates socket address structure
  bzero((char *)&socket_address, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(SERVER_PORT);
  //socket_address.sin_addr = *(struct in_addr *)host_address->h_addr;
  bcopy((char *)host_address->h_addr, (char *)&socket_address.sin_addr.s_addr, host_address->h_length);

  // Creates active socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Connects to server
  int ret = connect(server_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));

  if (ret >= 0) {
      log::write(DEBUG, "Socket " + std::to_string(server_fd) + " was connected");
  }
  else {
      log::write(FAIL, std::strerror(errno));
  }

  do {
    bzero(buf, sizeof(char)*MAX_LINE);
    std::getline(std::cin, str_in);
    len = str_in.size();
    
    int n = write(server_fd, str_in.c_str(), len);
    
    if (n <= 0) {
        log::write(FAIL, "Fail to write to server");
    }

    n = read(server_fd, buf, MAX_LINE);
    
    if (n <= 0) {
        log::write(FAIL, "Fail to read from server");
    }

    std::cout << std::string(buf) << std::endl;
  } while(std::strcmp(buf, "exit\r\n") && std::strcmp(buf, "exit\n"));

  close(server_fd);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
