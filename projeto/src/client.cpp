////////////////////////////////////////////////////////////////////////////////
/**
 * @file client.cpp
 * @date 2017-06-08
 * @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
 *
 * @copyright Tiago Lobato Gimenes 2017. All rights reserved.
 *
 * @brief
 *
 * This file contains implementation of the correspoding header file, i.e. .hpp,
 * .hh or .h
 */
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "client.hpp"
#include "car.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

client::client(int port, const std::string& ip, int delay) :
  port(port), ip(ip), delay(delay)
{
  is_waiting = false;
}

void client::send_car_info(car c)
{
  is_waiting = true;
}

car::action client::get_action()
{
  is_waiting = false;
  return car::action::KEEP;
}

void client::close_socket()
{
  // Closes socket
  close(socket_fd);
}

bool client::is_waiting_response()
{
  return is_waiting;
}

void client::start_delay()
{
  delay_start = std::chrono::high_resolution_clock::now();
  is_waiting = true;
}

void client::stop_delay()
{
  is_waiting = false;
}

int client::get_response_delay()
{
  std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time_span = now - delay_start;
  return time_span.count();
}

bool client::has_finished_delay()
{
  return (get_response_delay() > delay);
}

////////////////////////////////////////////////////////////////////////////////
