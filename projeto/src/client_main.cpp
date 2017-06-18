#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

#include "car.hpp"
#include "client.hpp"
#include "client_car.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

void create_client(std::vector<std::unique_ptr<client>> &clients, int port, std::string hostname, int delay, std::string protocol_str, std::string type_str)
{
  if (protocol_str == "tcp") {
    clients.emplace_back(new tcp_client(port, hostname, delay));
    log::write(DEBUG, "Starting " + type_str + " as TCP client");
  }
  else if (protocol_str == "udp") {
    clients.emplace_back(new udp_client(port, hostname, delay));
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
  if (argc != 8) {
    log::write(FAIL, "Usage: client_main hostname security_protocol entertainment_protocol comfort_protocol security_delay entertainment_delay comfort_delay");
  }
  std::string hostname(argv[1]);
  std::string security_protocol(argv[2]);
  std::string entertainment_protocol(argv[3]);
  std::string comfort_protocol(argv[4]);
  int security_delay = std::stoi(argv[5]);
  int entertainment_delay = std::stoi(argv[6]);
  int comfort_delay = std::stoi(argv[7]);

  // Initializes servers
  std::vector<std::unique_ptr<client>> clients;
  create_client(clients, SECURITY_PORT, hostname, security_delay, security_protocol, "security");
  create_client(clients, ENTERTAINMENT_PORT, hostname, entertainment_delay, entertainment_protocol, "entertainment");
  create_client(clients, COMFORT_PORT, hostname, comfort_delay, comfort_protocol, "comfort");

  // Randomizes car initial values
  int pos = random::range(0, 10);
  int speed = random::range(1, 3);
  car::direction dir = random::is_true() ? car::HORIZONTAL : car::VERTICAL;
  int size = 1;

  // Creates client car
  client_car cc(pos, speed, dir, size);

  while (true) {
    for (auto const &cli : clients) {
      // Sends own information to server
      if (!cli->is_waiting_response()) {
        cli->send_car_info(cc);
      }
      // Waits for action from server
      else if (cli->has_finished_delay()) {
        car::action ac = cli->get_action();
        cc.server_update(ac);
      }
      // Self updates position
      cc.self_update();
    }
  }

  return EXIT_SUCCESS;
}