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
    // Server protocol
    enum protocol {TCP, UDP};

    // Constructor
    server(int port, application type, int delay, protocol prot);

    virtual car get_car_info(); // Get action from client
    virtual void send_action(car::action ac); // Send action to client
    void close_socket(); // Close server socket

  protected:
    const int port;               // Server port
    const application type;       // Server application type
    const int delay;              // Server delay in milliseconds
    const protocol prot;          // Server protocol
    int socket_fd;                // Server socket file descriptor
    int client_fd;                // Current client fd
    struct sockaddr_in cli_addr;  // Current client address
    socklen_t cli_len;            // Current client address length
    char buf[MAX_LINE];           // Message buffer
};

////////////////////////////////////////////////////////////////////////////////

class tcp_server: public server
{
  public:
    // Constructor
    tcp_server(int port, application type, int delay);

    car get_car_info(); // Get action from client
    void send_action(car::action ac); // Send action to client

  private:
    int max_fd;
    int n_clients;
    int nready;
    int cli_index;
    int clients[FD_SETSIZE];
    fd_set new_set;
    fd_set all_fds;

    void tcp_socket();
    void tcp_bind();
    void tcp_listen();
    int tcp_accept();
    bool check_clients();
};

////////////////////////////////////////////////////////////////////////////////

class udp_server: public server
{
  public:
    // Constructor
    udp_server(int port, application type, int delay);

    car get_car_info(); // Get action from client
    void send_action(car::action ac); // Send action to client

  private:
    void udp_socket();
    void udp_bind();
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! SERVER_HPP */

////////////////////////////////////////////////////////////////////////////////
