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

#include <string>

////////////////////////////////////////////////////////////////////////////////

class client
{
  public:
    client(int port, const std::string& ip);
};

////////////////////////////////////////////////////////////////////////////////

class tcp_client: public client
{
  public:
    tcp_client(int port, const std::string& ip): client(port, ip) {}
};

////////////////////////////////////////////////////////////////////////////////

class udp_client: public client
{
  public:
    udp_client(int port, const std::string& ip): client(port, ip) {}
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! CLIENT_HPP */

////////////////////////////////////////////////////////////////////////////////
