#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <sys/socket.h>
#include <sys/select.h>
#include "server.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

server::server(int port, application type, int delay) :
  port(port), type(type), delay(delay)
{
}

////////////////////////////////////////////////////////////////////////////////

tcp_server::tcp_server(int port, application type, int delay) :
  server(port, type, delay)
{
  // Creates socket and get a file descriptor of the socket
  socket_fd = tcp_socket();

  // Binds the socket to the descriptor
  tcp_bind(socket_fd);

  //  Listen the socket for accepting connections
  tcp_listen(socket_fd);
}

car tcp_server::get_car_info()
{
  car new_car = car(0, 1, car::direction::HORIZONTAL, 1);
  return new_car;
}

void tcp_server::send_action(car::action act)
{

}

int tcp_server::tcp_socket()
{
  // Creates socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  
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

void tcp_server::tcp_bind(int socket)
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char *)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket, (struct sockaddr*) &address, sizeof(address));

  if (ret >= 0) { // On success, log
    log::write(DEBUG, "Socket " + std::to_string(socket) + " binded to port " + std::to_string(port) + " successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}

void tcp_server::tcp_listen(int socket)
{
  int ret = listen(socket, MAX_PENDING);

  if (ret >= 0) { // On success, log
    log::write(DEBUG, "Socket " + std::to_string(socket) + " is listenning for new connections");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}

int tcp_server::tcp_accept(int socket, struct sockaddr_in& address)
{
  socklen_t addrlen = sizeof(address);

  int fd = accept(socket, (struct sockaddr*) &address, &addrlen);

  if (fd >= 0) { // On success, log
    log::write(DEBUG, "New connection accepted: " + std::to_string(fd));
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  std::cout << "Client [" << inet_ntoa(address.sin_addr) << ":";
  std::cout << ntohs(address.sin_port) << "] connected!" << std::endl;
  
  return fd;
}

int tcp_server::tcp_read(int fd, char* buff)
{
  int n = read(fd, buff, MAX_LINE);
  
  if (n >= 0) { // On success, log
    log::write(DEBUG, "Received " + std::to_string(n) + " bytes");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }

  return n;
}

bool tcp_server::client_handler(int client_fd, struct sockaddr_in address)
{
  std::string ip = inet_ntoa(address.sin_addr);
  std::string port = std::to_string(ntohs(address.sin_port));
  char buf[MAX_LINE];

  // Clear the string
  bzero(buf, sizeof(char)*MAX_LINE);

  // Receives data from client at a max of MAX_LINE bytes and sets buffer buf
  int n = tcp_read(client_fd, buf);

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

////////////////////////////////////////////////////////////////////////////////

udp_server::udp_server(int port, application type, int delay) :
  server(port, type, delay)
{
  // Creates socket and get a file descriptor of the socket
  socket_fd = udp_socket();

  // Binds the socket to the descriptor
  udp_bind(socket_fd);
}

car udp_server::get_car_info()
{
  car new_car = car(0, 1, car::direction::HORIZONTAL, 1);
  return new_car;
}

void udp_server::send_action(car::action act)
{

}

int udp_server::udp_socket()
{
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

void udp_server::udp_bind(int socket)
{
  struct sockaddr_in address;

  // Initializes socket address
  bzero((char *)&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int ret = bind(socket, (struct sockaddr*) &address, sizeof(address));

  if(ret >= 0) { // On success, log 
    log::write(DEBUG, "Socket " + std::to_string(socket) + " binded to port " + std::to_string(port) + " successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}
