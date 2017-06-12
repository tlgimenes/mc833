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

server::server(int port, application type, int delay, protocol prot) :
  port(port), type(type), delay(delay), prot(prot)
{
}

car server::get_car_info()
{
  car new_car = car(0, 1, car::direction::HORIZONTAL, 1);
  return new_car;
}

void server::send_action(car::action ac)
{
}

void server::close_socket()
{
  // Closes socket
  close(socket_fd);
}

////////////////////////////////////////////////////////////////////////////////

tcp_server::tcp_server(int port, application type, int delay) :
  server(port, type, delay, server::protocol::TCP)
{
  // Creates socket and get a file descriptor of the socket
  socket_fd = tcp_socket();

  // Binds the socket to the descriptor
  tcp_bind(socket_fd);

  // Listens the socket for accepting connections
  tcp_listen(socket_fd);

  // Initializes variables
  n_clients = -1;
  for (int i = 0; i < FD_SETSIZE; i++) {
    clients[i] = -1;
  }
  FD_ZERO(&all_fds);
  cli_index = 0;

  // Adds server socket to the set
  max_fd = socket_fd;
  FD_SET(socket_fd, &all_fds);
}

car tcp_server::get_car_info()
{
  while (true) {
    // Wait until some client is ready
    if (cli_index > n_clients && !check_clients()) {
      continue;
    }

    // Verifies which clients have data to be read and process it
    for (int i = cli_index; i <= n_clients; i++) {
      // Gets valid client file descriptor
      client_fd = clients[i];
      if (client_fd < 0) continue;

      // Checks if client has data
      if (FD_ISSET(client_fd, &new_set)) {
        // Clears the string
        bzero(buf, sizeof(char) * MAX_LINE);

        // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
        int n = read(client_fd, buf, MAX_LINE);

        // Checks success or failure
        if (n >= 0) {
          log::write(DEBUG, "Received " + std::to_string(n) + " bytes");
        }
        else {
          log::write(FAIL, std::strerror(errno));
        }

        // Closes connection if client has disconnected
        if (n == 0) {
          close(client_fd);
          FD_CLR(client_fd, &all_fds);
          clients[i] = -1;
          log::write(DEBUG, "Socket " + std::to_string(client_fd) + " has been disconnected");
        }
        // Or process string otherwise
        else {
          // Creates car from string
          std::string car_str(buf);
          car new_car = car(car_str);

          // Returns client car
          return new_car;
        }

        // There aren't and file descriptors to be read
        if (--nready <= 0) break;
      }
    }
  }

  car new_car = car(0, 1, car::direction::HORIZONTAL, 1);
  return new_car;
}

bool tcp_server::check_clients()
{
  // Creates copy of all_fds
  new_set = all_fds;

  // Checks how many clients have data
  nready = select(max_fd + 1, &new_set, NULL, NULL, NULL);

  // Exits in case of error on select
  if (nready < 0) {
    log::write(FAIL, std::strerror(errno));
  }

  // Checks if new client is trying to connect
  if (FD_ISSET(socket_fd, &new_set)) {
    // Accepts new connection
    int i, fd = tcp_accept(socket_fd);

    // Saves file descriptor on vector
    for (i = 0; i < FD_SETSIZE; i++) {
      if (clients[i] < 0) {
        clients[i] = fd;
        break;
      }
    }

    // Disconnects client if max number of clients is reached
    if (i == FD_SETSIZE) {
      close(fd);
    }

    // Adds client file descriptor to the set
    FD_SET(fd, &all_fds);

    if (fd > max_fd)
      max_fd = fd; // Updates max file descriptor
    if (i > n_clients)
      n_clients = i; // Updates max index on vector
    if (--nready <= 0)
      return false; // There aren't and file descriptors to be read
  }

  return true;
}

void tcp_server::send_action(car::action ac)
{
  // Send action to car as string
  std::string ac_str = car::action_to_string(ac);
  strncpy(buf, ac_str.c_str(), MAX_LINE);
  int str_len = sizeof(char) * ac_str.length();
  int n = write(client_fd, buf, str_len);

  // Checks success or failure
  if (n >= 0) {
    log::write(DEBUG, "Wrote " + std::to_string(n) + " bytes to client");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

int tcp_server::tcp_socket()
{
  // Creates socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  
  // Sets option to reuse socket 
  int optval = 1;
  int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  // Checks success or failure
  if (fd >= 0 && opt >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(fd) + " created successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

void tcp_server::tcp_bind(int socket)
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char *)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket, (struct sockaddr*)&address, sizeof(address));

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket) + " binded to port " + std::to_string(port) + " successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void tcp_server::tcp_listen(int socket)
{
  int ret = listen(socket, MAX_PENDING);

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket) + " is listenning for new connections");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

int tcp_server::tcp_accept(int socket)
{
  int fd = accept(socket, (struct sockaddr*)&cli_addr, &cli_len);

  // Checks success or failure
  if (fd >= 0) {
    log::write(DEBUG, "New connection accepted: " + std::to_string(fd));
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

////////////////////////////////////////////////////////////////////////////////

udp_server::udp_server(int port, application type, int delay) :
  server(port, type, delay, server::protocol::UDP)
{
  // Creates socket and get a file descriptor of the socket
  socket_fd = udp_socket();

  // Binds the socket to the descriptor
  udp_bind(socket_fd);
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

int udp_server::udp_socket()
{
  // Creates socket
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  
  // Sets option to reuse socket 
  int optval = 1;
  int opt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  // Checks success or failure
  if (fd >= 0 && opt >= 0) {
    log::write(DEBUG, "UDP socket " + std::to_string(fd) + " created successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}

void udp_server::udp_bind(int socket)
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char*)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket, (struct sockaddr*) &address, sizeof(address));

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket) + " binded to port " + std::to_string(port) + " successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}
