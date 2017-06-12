#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "server.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

udp_server::udp_server(int port, application type, int delay) :
  server(port, type, delay, server::protocol::UDP)
{
  // Creates socket and get a file descriptor of the socket
  udp_socket();

  // Binds the socket to the descriptor
  udp_bind();
}

car udp_server::get_car_info()
{
  while (true) {
    // Clears the buffer
    int str_len = sizeof(char) * MAX_LINE;
    bzero(buf, str_len);

    // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
    ssize_t n = recvfrom(socket_fd, buf, str_len, 0, (struct sockaddr*)&cli_addr, &cli_len);

    // If didn't receive anything or something bad happened to the client
    if (n == 0) continue;

    // Creates car from string
    std::string car_str(buf);
    car new_car = car(car_str);

    // Returns client car
    return new_car;
  }
}

void udp_server::send_action(car::action ac)
{
  // Send action to car as string
  std::string ac_str = car::action_to_string(ac);
  strncpy(buf, ac_str.c_str(), MAX_LINE);
  int str_len = sizeof(char) * ac_str.length();
  ssize_t n = sendto(socket_fd, buf, str_len, 0, (const struct sockaddr *)&cli_addr, cli_len);

  // Checks success or failure
  if (n >= 0) {
    log::write(DEBUG, "Wrote " + std::to_string(n) + " bytes to client");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void udp_server::udp_socket()
{
  // Creates socket
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  
  // Sets option to reuse socket 
  int optval = 1;
  int opt = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  // Checks success or failure
  if (socket_fd >= 0 && opt >= 0) {
    log::write(DEBUG, "UDP socket " + std::to_string(socket_fd) + " created successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void udp_server::udp_bind()
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char*)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket_fd, (struct sockaddr*) &address, sizeof(address));

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " binded to port " + std::to_string(port) + " successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}
