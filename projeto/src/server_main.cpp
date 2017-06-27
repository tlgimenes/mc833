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
  // Sets verbose level. If compiled in debug mode, show all messages, else show only FAIL errors
#ifdef NDEBUG
  log::level() = FAIL;
#else
  log::level() = DEBUG;
#endif

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
  int port = SECURITY_PORT;
  server::application type = server::application::SECURITY;
  bool ignore_prediction = false;
  if (type_str == "security") {
    port = SECURITY_PORT;
    type = server::application::SECURITY;
  }
  else if (type_str == "entertainment") {
    port = ENTERTAINMENT_PORT;
    type = server::application::ENTERTAINMENT;
    ignore_prediction = true;
  }
  else if (type_str == "comfort") {
    port = COMFORT_PORT;
    type = server::application::COMFORT;
    ignore_prediction = true;
  }
  else {
    log::write(FAIL, "Invalid type: use 'security', 'entertainment' or 'comfort'");
  }

  // Initializes predictor
  predictor pred(50, 60, 60, 74, ignore_prediction);

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

  std::vector<std::pair<int, car::action>> actions;

  // Main server loop
  while (true) {
    // Gets next car information from server
    car c = srv->get_car_info();

    // Adds to predictor if it doens't exist
    if (!pred.exists(c)) {
      pred.add_car(c);
    }

    // Updates predictor and gets action
    pred.update(actions, c);

    // Draws prediction if is security server
    if (type == server::application::SECURITY) {
      pred.draw();
    }

    // Sends action to car
    srv->send_action(actions);
    
    // Clears actions
    actions.clear();
  }

  // Close server sockets
  srv->close_socket();

  return EXIT_SUCCESS;
}
