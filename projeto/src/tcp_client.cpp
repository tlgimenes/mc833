#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "client.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

tcp_client::tcp_client(int port, const std::string& ip) :
  client(port, ip)
{
  host = gethostbyname(ip.c_str());

  // In case no host is found
  if (host == NULL) {
    log::write(FAIL, "No host found with given name");
  }

  // Creates active socket
  tcp_socket();

  // Connects socket to server
  tcp_connect();
}

void tcp_client::send_car_info(car c)
{
    // Clears the string
    bzero(buf, sizeof(char) * MAX_LINE);

    // Send car as string
    std::string car_str = c.to_string();
    int str_len = car_str.size();
    int n = write(socket_fd, car_str.c_str(), str_len);

    // Checks success or failure
    if (n <= 0) {
      log::write(FAIL, "Fail to write to server");
    }
    is_waiting = true;
    log::write(DEBUG, "Sent car to server: " + car_str);
}

car::action tcp_client::get_action()
{
  // Reads server
  int n = read(socket_fd, buf, MAX_LINE);

  // Checks success or failure
  if (n <= 0) {
    log::write(FAIL, "Fail to read from server");
  }
  is_waiting = false;

  // Gets action from string
  std::string ac_str(buf);
  car::action ac = car::string_to_action(ac_str);
  log::write(DEBUG, "Received action from server: " + ac_str);

  return ac;
}

void tcp_client::tcp_socket()
{
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Sets option to reuse socket
  int optval = 1;
  int opt = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  // Checks success or failure
  if (socket_fd >= 0 && opt >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " created successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void tcp_client::tcp_connect()
{
  // Creates socket address structure
  bzero((char *)&srv_addr, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(port);
  bcopy((char *)host->h_addr, (char *)&srv_addr.sin_addr.s_addr, host->h_length);

  // Connects to server
  int ret = connect(socket_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " was connected");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}