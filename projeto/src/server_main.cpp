#include <vector>
#include <string>

#include "car.hpp"
#include "server.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

void instantiate_server(std::vector<server> &servers, std::string protocol, int port, server::application type, int delay)
{
  if (protocol == "tcp") {
    servers.push_back(tcp_server(port, type, delay));
  }
  else if (protocol == "udp") {
    servers.push_back(udp_server(port, type, delay));
  }
  else {
    log::write(FAIL, "Invalid protocol: use 'tcp' or 'udp'");
  }
}

int main(int argc, char** argv) 
{
  // Checks given arguments
  if (argc != 3) {
    log::write(FAIL, "Usage: server_main security_protocol entertainment_protocol comfort_protocol");
  }

  std::vector<server> servers;
  instantiate_server(servers, argv[1], SECURITY_PORT, server::application::SECURITY, 10);
  instantiate_server(servers, argv[2], ENTERTAINMENT_PORT, server::application::ENTERTAINMENT, 100);
  instantiate_server(servers, argv[3], COMFORT_PORT, server::application::COMFORT, 100);

  return EXIT_SUCCESS;
}