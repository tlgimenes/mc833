////////////////////////////////////////////////////////////////////////////////
/**
 * @file client.hpp
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

#ifndef CLIENT_HPP
#define CLIENT_HPP

////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <ratio>  
#include <string>
#include <arpa/inet.h>
#include "car.hpp"

////////////////////////////////////////////////////////////////////////////////

#define MAX_LINE 256
#define SECURITY_PORT 12345
#define ENTERTAINMENT_PORT 12346
#define COMFORT_PORT 12347

////////////////////////////////////////////////////////////////////////////////

class client
{
  public:
    // Constructor
    client(int port, const std::string& ip, int delay);

    virtual void send_car_info(car c);  // Send car info to server
    virtual car::action get_action();   // Get action from server
    void close_socket();                // Close client socket
    bool is_waiting_response();         // Is client waiting for server response
    int get_response_delay();           // Get server response delay in ms
    bool has_finished_delay();          // Has server delay time finished

  protected:
    const int port;               // Server port
    const std::string ip;         // Server IP
    const int delay;              // Server delay in ms
    int socket_fd;                // Client socket file descriptor
    char buf[MAX_LINE];           // Message buffer
    struct hostent *host;         // Server host name
    struct sockaddr_in srv_addr;  // Server address
    std::chrono::high_resolution_clock::time_point delay_start; // When delay count started

    void start_delay(); // Start counting delay
    void stop_delay();  // Stop counting delay

  private:
    bool is_waiting;  // Client is waiting for server response
};

////////////////////////////////////////////////////////////////////////////////

class tcp_client: public client
{
  public:
    tcp_client(int port, const std::string& ip, int delay);

    void send_car_info(car c);  // Send car info to server
    car::action get_action();   // Get action from server

  private:
    void tcp_socket();
    void tcp_connect();
};

////////////////////////////////////////////////////////////////////////////////

class udp_client: public client
{
  public:
    udp_client(int port, const std::string& ip, int delay);

    void send_car_info(car c);  // Send car info to server
    car::action get_action();   // Get action from server

  private:
    void udp_socket();
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! CLIENT_HPP */

////////////////////////////////////////////////////////////////////////////////
