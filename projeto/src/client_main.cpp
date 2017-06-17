#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

#include "car.hpp"
#include "client.hpp"
#include "client_car.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

void create_client(std::vector<std::unique_ptr<client>> &clients, int port, std::string hostname, std::string protocol_str, std::string type_str)
{
  if (protocol_str == "tcp") {
    clients.emplace_back(new tcp_client(port, hostname));
    log::write(DEBUG, "Starting " + type_str + " as TCP client");
  }
  else if (protocol_str == "udp") {
    clients.emplace_back(new udp_client(port, hostname));
    log::write(DEBUG, "Starting " + type_str + " as UDP client");
  }
  else if (protocol_str == "none") {
    log::write(DEBUG, "Skipping " + type_str + " client");
  }
  else {
    log::write(FAIL, "Invalid protocol: use 'tcp', 'udp' or 'none'");
  }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  // Sets verbose level. If compiled in debug mode, show all messages, else show only FAIL errors 
#ifdef NDEBUG
  log::level() = DEBUG;
#else
  log::level() = FAIL;
#endif

  // Checks given arguments
  if (argc != 5) {
    log::write(FAIL, "Usage: client_main hostname security_protocol entertainment_protocol comfort_protocol");
  }
  std::string hostname(argv[1]);
  std::string security_protocol(argv[2]);
  std::string entertainment_protocol(argv[3]);
  std::string comfort_protocol(argv[4]);

  // Initializes servers
  std::vector<std::unique_ptr<client>> clients;
  create_client(clients, SECURITY_PORT, hostname, security_protocol, "security");
  create_client(clients, ENTERTAINMENT_PORT, hostname, entertainment_protocol, "entertainment");
  create_client(clients, COMFORT_PORT, hostname, comfort_protocol, "comfort");

  // Creates client car
  client_car cc(20, 2, car::HORIZONTAL);

  while (true) {
    for (auto const &cli : clients) {
      if (cli->is_waiting) {
        car::action ac = cli->get_action();
        cc.server_update(ac);
      }
      else {
        cli->send_car_info(cc);
      }
    }
  }

  return EXIT_SUCCESS;
}