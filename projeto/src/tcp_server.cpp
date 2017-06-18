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

tcp_server::tcp_server(int port, application type, int delay) :
  server(port, type, delay, server::protocol::TCP)
{
  // Creates socket and get a file descriptor of the socket
  tcp_socket();

  // Binds the socket to the descriptor
  tcp_bind();

  // Listens the socket for accepting connections
  tcp_listen();

  // Initializes variables
  n_clients = 0;
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
    // Resets client array index counter
    if (cli_index >= n_clients) {
      if (!check_clients()) {
        continue;
      }
      else {
        cli_index = 0;
      }
    }

    // Verifies which clients have data to be read and process it
    for (; cli_index < n_clients; cli_index++) {
      // Gets valid client file descriptor
      client_fd = clients[cli_index];
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
        // Closes connection if client has disconnected
        else if (errno == ECONNRESET) {
          close(client_fd);
          FD_CLR(client_fd, &all_fds);
          clients[cli_index] = -1;
          log::write(DEBUG, "Socket " + std::to_string(client_fd) + " has been disconnected");
          continue;
        }
        else {
          log::write(FAIL, std::strerror(errno));
        }

        // Creates car from string
        std::string car_str(buf);
        car new_car = car(car_str);
        log::write(DEBUG, "Received car from client: " + car_str);

        // Returns client car
        cli_index++;
        return new_car;

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
    int i, fd = tcp_accept();

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
    if (i + 1 > n_clients)
      n_clients = i + 1; // Updates max index on vector
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
    log::write(DEBUG, "Sent action to client: " + ac_str);
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void tcp_server::tcp_socket()
{
  // Creates socket
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

void tcp_server::tcp_bind()
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char *)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket_fd, (struct sockaddr*)&address, sizeof(address));

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " binded to port " + std::to_string(port) + " successfully");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

void tcp_server::tcp_listen()
{
  int ret = listen(socket_fd, MAX_PENDING);

  // Checks success or failure
  if (ret >= 0) {
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " is listenning for new connections");
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }
}

int tcp_server::tcp_accept()
{
  cli_len = sizeof(cli_addr);
  int fd = accept(socket_fd, (struct sockaddr*)&cli_addr, &cli_len);

  // Checks success or failure
  if (fd >= 0) {
    log::write(DEBUG, "New connection accepted: " + std::to_string(fd));
  }
  else {
    log::write(FAIL, std::strerror(errno));
  }

  return fd;
}
