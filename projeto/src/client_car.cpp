////////////////////////////////////////////////////////////////////////////////
/**
 * @file client_car.cpp
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

#include "client_car.hpp"
#include "utils.hpp"

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

client_car::client_car(int pos, int speed, direction dir, int size) :
  car(pos, speed, dir, size)
{
  last_update = std::chrono::high_resolution_clock::now();
  std::string filename = LOG_FILE_PREFIX + std::to_string(id);
  log_file.open("test.txt", std::fstream::out);
}

bool client_car::server_update(action ac)
{
  switch (ac) {
    case car::action::ACCEL: speed += accel; break;
    case car::action::BREAK: speed -= accel; break;
    case car::action::AMBULANCE: return true;
    case car::action::KEEP: default: break;
  }
  return false;
}

void client_car::self_update()
{
  // Calculates delta update time
  std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> delta_update = now - last_update;
  last_update = now;

  // Randomizes speed with 1:100 chance
  if (random::is_true(100)) {
    speed += random::range(-accel, accel);
    speed = std::max(0, speed);
    speed = std::min(MAX_SPEED, speed);
  }

  // Calculates delta position
  delta_pos += speed * delta_update.count();
  if (delta_pos > 1) {
    int int_delta_pos = (int)delta_pos;
    delta_pos -= int_delta_pos;
    pos += int_delta_pos;
  }
}

////////////////////////////////////////////////////////////////////////////////
