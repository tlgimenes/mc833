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

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

client_car::client_car(int pos, int speed, direction dir, int size) :
  car(pos, speed, dir, size)
{ 
}

void client_car::server_update(action ac)
{

}

void client_car::self_update()
{
  this->speed += rand() % (2*car::accel+1) - car::accel;
  this->speed = std::max(0, speed);
}

////////////////////////////////////////////////////////////////////////////////
