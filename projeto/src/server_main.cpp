#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

#include "car.hpp"
#include "server.hpp"
#include "predictor.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  // Checks given arguments
  if (argc != 4) {
    log::write(FAIL, "Usage: server_main type protocol delay");
  }
  std::string type_str(argv[1]);
  std::string protocol_str(argv[2]);
  int delay = std::stoi(argv[3]);

  // Checks delay value
  if (delay < 0) {
    delay = 0;
    log::write(WARNING, "Delay can't be negative so 0 will be used");
  }

  // Gets correct port and protocol type
  int port;
  server::application type;
  if (type_str == "security") {
    port = SECURITY_PORT;
    type = server::application::SECURITY;
  }
  else if (type_str == "entertainment") {
    port = ENTERTAINMENT_PORT;
    type = server::application::ENTERTAINMENT;
  }
  else if (type_str == "comfort") {
    port = COMFORT_PORT;
    type = server::application::COMFORT;
  }
  else {
    log::write(FAIL, "Invalid type: use 'security', 'entertainment' or 'comfort'");
  }

  // Initializes TCP or UDP server
  std::unique_ptr<server> srv;
  if (protocol_str == "tcp") {
    srv.reset(new tcp_server(port, type, delay));
    log::write(DEBUG, "Starting " + type_str + " as TCP server");
  }
  else if (protocol_str == "udp") {
    srv.reset(new udp_server(port, type, delay));
    log::write(DEBUG, "Starting " + type_str + " as UDP server");
  }
  else if (protocol_str == "none") {
    log::write(DEBUG, "Skipping " + type_str + " server");
    return EXIT_SUCCESS;
  }
  else {
    log::write(FAIL, "Invalid protocol: use 'tcp', 'udp' or 'none'");
  }

  // Initializes predictor
  predictor pred(50, 60, 60, 74);

  // Main server loop
  while (true) {
    // Gets next car information from server
    car c = srv->get_car_info();

    // Adds to predictor if it doens't exist
    if (!pred.exists(c)) {
      pred.add_car(c);
    }

    // Updates predictor and gets action
    car::action ac = pred.update(c);

    // Sends action to car
    srv->send_action(ac);
  }

  // Close server sockets
  srv->close_socket();

  return EXIT_SUCCESS;
}