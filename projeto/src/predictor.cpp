////////////////////////////////////////////////////////////////////////////////
/**
 * @file predictor.cpp
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

#include "predictor.hpp"

#include <cassert>
#include <vector>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

predictor::predictor(int cross_h0, int cross_hf, int cross_v0, int cross_vf) : 
  cross_h0(cross_h0), cross_hf(cross_hf), cross_v0(cross_v0), cross_vf(cross_vf), cars()
{
  assert(cross_h0 < cross_hf && cross_v0 < cross_vf);
  assert(cross_hf < max_h && cross_vf < max_v);
}

////////////////////////////////////////////////////////////////////////////////

void predictor::add_car(const car& c) 
{
  cars.insert(std::pair<int, car>(c.id, c));
}

////////////////////////////////////////////////////////////////////////////////

bool predictor::exists(const car& c) const 
{
  return cars.find(c.id) != cars.end();
}

////////////////////////////////////////////////////////////////////////////////

car::action predictor::update(const car& c) 
{
  std::vector<std::pair<float,int>> t0s;
  std::vector<std::pair<float,int>> tfs;

  // Update car values
  this->cars.find(c.id)->second = c;

  t0s.reserve(cars.size());
  tfs.reserve(cars.size());

  for(auto& car: cars) {
    float t0, tf;
    this->intersects(t0, tf, car.second);
    t0s.push_back(std::make_pair(t0, car.second.id));
    tfs.push_back(std::make_pair(tf, car.second.id));
  }

  return car::ACCEL;
}

////////////////////////////////////////////////////////////////////////////////

void predictor::intersects(float &t0, float &tf, const car& c)
{
  t0 = (cross_v0 - c.pos) / c.speed;
  tf = (cross_vf + c.size - c.pos) / c.speed;
}

////////////////////////////////////////////////////////////////////////////////

void predictor::draw()
{
  float screen_h = 20, screen_v = 20;
  float scale_h = screen_h / max_h, scale_v = screen_v / max_v;

  std::vector<std::vector<char>> map2D(screen_v);
  for(auto& line: map2D) line.resize(screen_h, ' ');

  map2D[cross_v0*scale_v].assign(screen_h, '-');
  map2D[cross_vf*scale_v].assign(screen_h, '-');
  for(auto& line: map2D) {
    line[cross_h0*scale_h] = '|';
    line[cross_hf*scale_h] = '|';
  }

  for(auto& car: cars) {
    if(car.second.dir == car::HORIZONTAL) {
      assert(car.second.pos < max_h);
      map2D[cross_v0*scale_v+1][car.second.pos*scale_h] = '#';
    }
    else {
      assert(car.second.pos < max_v);
      map2D[car.second.pos*scale_v][cross_h0*scale_h+1] = '#';
    }
  }

  for(auto& line: map2D) {
    for(auto& elem: line) {
      std::cout << elem;
    } 
    std::cout << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
