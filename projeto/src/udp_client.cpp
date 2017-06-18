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

udp_client::udp_client(int port, const std::string& ip, int delay) :
  client(port, ip, delay)
{
  host = gethostbyname(ip.c_str());

  // In case no host is found
  if (host == NULL) {
    log::write(FAIL, "No host found with given name");
  }

  // Creates active socket
  udp_socket();

  // Gets server address
  bzero((char *)&srv_addr, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(port);
  bcopy((char *)host->h_addr, (char *)&srv_addr.sin_addr.s_addr, host->h_length);
}

void udp_client::send_car_info(car c)
{
    // Clears the string
    bzero(buf, sizeof(char) * MAX_LINE);

    // Send car as string
    std::string car_str = c.to_string();
    int str_len = car_str.size();
    int n = sendto(socket_fd, car_str.c_str(), str_len, 0, (const struct sockaddr*)&srv_addr, sizeof(srv_addr));

    // In case it fails to write
    if (n <= 0) {
      log::write(FAIL, strerror(errno));
    }
    start_delay();
    log::write(DEBUG, "Sent car to server: " + car_str);
}

car::action udp_client::get_action()
{
  // Reads server
  int n = recvfrom(socket_fd, buf, MAX_LINE, 0, NULL, NULL);

  // Checks success or failure
  if (n <= 0) {
    log::write(FAIL, "Fail to receive from server");
  }
  stop_delay();

  // Gets action from string
  std::string ac_str(buf);
  car::action ac = car::string_to_action(ac_str);
  std::string delay_str = std::to_string(get_response_delay());
  log::write(DEBUG, "Received action from server in " + delay_str + "ms: " + ac_str);

  return ac;
}

void udp_client::udp_socket()
{
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  // Sets option to reuse socket
  int optval = 1;
  int opt = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (socket_fd >= 0 && opt >= 0) { // On success, print new file descriptor
    log::write(DEBUG, "Socket " + std::to_string(socket_fd) + " created successfully");
  }
  else { // On fail, print error
    log::write(FAIL, std::strerror(errno));
  }
}
