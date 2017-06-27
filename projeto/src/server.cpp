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

void server::send_action(std::vector<std::pair<int, car::action>>& acs)
{
  log::write(FAIL, "Not Implemented Error");
}

void server::close_socket()
{
  // Closes socket
  close(socket_fd);
}
