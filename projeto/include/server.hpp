////////////////////////////////////////////////////////////////////////////////
/**
 * @file server.hpp
 * @date 2017-06-08
 * @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
 * @author Renato Vargas
 *
 * @copyright Tiago Lobato Gimenes 2016. All rights reserved.
 *
 * @brief
 *
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef SERVER_HPP
#define SERVER_HPP

////////////////////////////////////////////////////////////////////////////////

#include <arpa/inet.h>
#include "car.hpp"

////////////////////////////////////////////////////////////////////////////////

#define MAX_PENDING 5
#define MAX_LINE 256
#define SECURITY_PORT 12345
#define ENTERTAINMENT_PORT 12346
#define COMFORT_PORT 12347

////////////////////////////////////////////////////////////////////////////////

class server
{
  public:
    // Application type of server
    enum application {SECURITY, ENTERTAINMENT, COMFORT};

    // Constructor
    server(int port, application type, int delay);

    virtual car get_car_info(); // Get action from client
    virtual void send_action(car::action act); // Send action to client

  protected:
    const int port;         // Server port
    const application type; // Server application type
    const int delay;        // Server delay in milliseconds
    int socket_fd;          // Server socket file descriptor
};

////////////////////////////////////////////////////////////////////////////////

class tcp_server: public server
{
  public:
    // Constructor
    tcp_server(int port, application type, int delay);

    car get_car_info(); // Get action from client
    void send_action(car::action act); // Send action to client

  private:
    int n_clients = -1;

    int tcp_socket();
    void tcp_bind(int socket);
    void tcp_listen(int socket);
    int tcp_accept(int socket, struct sockaddr_in& address);
    int tcp_read(int fd, char* buff);
    bool client_handler(int client_fd, struct sockaddr_in address);
};

////////////////////////////////////////////////////////////////////////////////

class udp_server: public server
{
  public:
    // Constructor
    udp_server(int port, application type, int delay);

    car get_car_info(); // Get action from client
    void send_action(car::action act); // Send action to client

  private:
    int udp_socket();
    void udp_bind(int socket);
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! SERVER_HPP */

////////////////////////////////////////////////////////////////////////////////
