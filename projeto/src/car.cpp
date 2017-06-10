////////////////////////////////////////////////////////////////////////////////
/**
 * @file car.cpp
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

#include "car.hpp"

#include <sstream>
#include <cstdlib>
#include <chrono>

////////////////////////////////////////////////////////////////////////////////

const int car::invalid_id = -1;

int car::ncars = 0;

////////////////////////////////////////////////////////////////////////////////

car::car(int pos, int speed, car::direction dir, int size): 
  dir(dir), pos(pos), speed(speed), size(size)
{
  
  if(!car::ncars) srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  id = rand();
  car::ncars++;
}

////////////////////////////////////////////////////////////////////////////////

car::car(const car& c) :
  pos(c.pos), speed(c.speed), id(c.id), dir(c.dir), size(c.size)
{
}

////////////////////////////////////////////////////////////////////////////////

car::car(const std::string& str)
{
  std::stringstream sstr(str);
  int sdir;

  sstr >> sdir >> pos >> speed >> id >> size;

  dir = (car::direction)sdir;
}

////////////////////////////////////////////////////////////////////////////////

std::string car::to_string()
{
  std::stringstream sstr;

  sstr << dir << " " << pos << " " << speed << " " << id << " " << size;

  return sstr.str();
}

////////////////////////////////////////////////////////////////////////////////
    
car& car::operator=(const car& c)
{
  this->id = c.id;
  this->speed = c.speed;
  this->pos = c.pos;
  this->dir = c.dir;
  this->size = c.size;

  return *this;
}

////////////////////////////////////////////////////////////////////////////////

bool car::operator==(const car& c)
{
  return this->id == c.id && this->size == c.size && this->speed == c.speed && this->pos == c.pos && this->dir == c.dir;
}

////////////////////////////////////////////////////////////////////////////////
