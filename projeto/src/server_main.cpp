#include "car.hpp"
#include "server.hpp"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) 
{
  udp_server security_server = udp_server(123, server::application::SECURITY, 10);
  tcp_server entertainment_server = tcp_server(123, server::application::ENTERTAINMENT, 100);
  tcp_server comfort_server = tcp_server(123, server::application::COMFORT, 100);
  return EXIT_SUCCESS;
}