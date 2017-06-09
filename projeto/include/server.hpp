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

class server
{
  public:
    server(int port);
};

////////////////////////////////////////////////////////////////////////////////

class tcp_server: public server
{
  public:
    tcp_server(int port): server(port) {}
};

////////////////////////////////////////////////////////////////////////////////

class udp_server: public server
{
  public:
    udp_server(int port): server(port) {}
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! SERVER_HPP */

////////////////////////////////////////////////////////////////////////////////
