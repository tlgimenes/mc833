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
#include "utils.hpp"

#include <cassert>
#include <vector>
#include <iostream>
#include <climits>

////////////////////////////////////////////////////////////////////////////////

#define INF 1e6

////////////////////////////////////////////////////////////////////////////////

predictor::predictor(int cross_h0, int cross_hf, int cross_v0, int cross_vf, bool always_keep) : 
  cross_h0(cross_h0), cross_hf(cross_hf), cross_v0(cross_v0), cross_vf(cross_vf), always_keep(always_keep)
{
#ifdef NDEBUG
  std::cout << "\033[?25l\033[2J" << std::endl;
#endif

  assert(cross_h0 < cross_hf && cross_v0 < cross_vf);
  assert(cross_hf < max_h && cross_vf < max_v);
}

////////////////////////////////////////////////////////////////////////////////

void predictor::add_car(const car& c) 
{
  float t0, tf;

  if(c.dir == car::HORIZONTAL) cars_h.insert(std::make_pair(c.id, c));
  else cars_v.insert(std::make_pair(c.id, c));

  intersects(t0, tf, c);

  t0s.insert(std::make_pair(c.id, t0));
  tfs.insert(std::make_pair(c.id, tf));
}

////////////////////////////////////////////////////////////////////////////////

void predictor::remove_car(const car& c) 
{
  if(c.dir == car::HORIZONTAL) cars_h.erase(cars_h.find(c.id));
  else cars_v.erase(cars_v.find(c.id));

  t0s.erase(t0s.find(c.id));
  tfs.erase(tfs.find(c.id));
}

////////////////////////////////////////////////////////////////////////////////

void predictor::remove_car(int car_id) 
{
  auto it = cars_h.find(car_id);

  if(it != cars_h.end()) cars_h.erase(it);
  else if((it=cars_v.find(car_id)) != cars_v.end()) cars_v.erase(it);
  else
    log::write(FAIL, "Fail to delete car "+std::to_string(car_id));

  t0s.erase(t0s.find(car_id));
  tfs.erase(tfs.find(car_id));
}

////////////////////////////////////////////////////////////////////////////////


bool predictor::exists(const car& c) const 
{
  if(c.dir == car::HORIZONTAL) return cars_h.find(c.id) != cars_h.end();

  return cars_v.find(c.id) != cars_v.end();
}

////////////////////////////////////////////////////////////////////////////////

inline bool are_intervals_disjoint(float I00, float I0f, float I10, float I1f) 
{
  assert(I00 < I0f && I10 < I1f);

  return (I0f < I10 || I1f < I00);
}

////////////////////////////////////////////////////////////////////////////////

enum predictor::position_level_t predictor::position_level(const car& c)
{
  int x0=0, xx0, xxf, xf;
  if(c.dir == car::HORIZONTAL) { 
    xx0 = cross_h0; 
    xxf = cross_hf; 
    xf = max_h; 
  }
  else {
    xx0 = cross_v0; 
    xxf = cross_vf; 
    xf = max_v; 
  }

  if(c.pos < x0) return predictor::OUT_OF_BOUNDS;
  if(x0 <= c.pos && c.pos <= xx0) return predictor::BEFORE_CROSSING;
  if(xx0 < c.pos && c.pos < c.size + xxf) return predictor::IN_CROSSING;
  if(xxf + c.size <= c.pos && c.pos < xf) return predictor::AFTER_CROSSING;
  if(c.pos >= xf) return predictor::OUT_OF_BOUNDS;

  return predictor::OUT_OF_BOUNDS;
}

////////////////////////////////////////////////////////////////////////////////

void predictor::update(std::vector<std::pair<int, car::action>>& ac, car& c) 
{
  auto& array_c_belongs = (c.dir == car::HORIZONTAL) ? cars_h : cars_v;
  auto& array_c_not_belongs = (c.dir == car::HORIZONTAL) ? cars_v : cars_h;
  std::vector<float> t0(car::accel*2+1), tf(car::accel*2+1);
  std::vector<float> t0_col, tf_col;
  int speed0 = c.speed;
  auto pl = position_level(c);

  // Car need to be on the array
  assert(array_c_belongs.find(c.id) != array_c_belongs.end());
  // Updates car C's array
  array_c_belongs.find(c.id)->second = c;

  if (always_keep) {// Check if should always send keep action
    ac.push_back(std::make_pair(c.id, car::KEEP));
    return;
  }
  if (pl == predictor::OUT_OF_BOUNDS) { // If after map, disconnect
    remove_car(c);
    log::write(DEBUG, "Disconnect car "+std::to_string(c.id));
    ac.push_back(std::make_pair(c.id, car::DISCONNECT));
    return;
  }

  if(pl == predictor::IN_CROSSING) {
    for(auto& cr: array_c_not_belongs) {
      if(position_level(cr.second) == predictor::IN_CROSSING) {
        ac.push_back(std::make_pair(cr.second.id, car::AMBULANCE));
        remove_car(cr.second.id);
      }
    }
    if(ac.size() > 0) {
      ac.push_back(std::make_pair(c.id, car::AMBULANCE));
      remove_car(c);
      return;
    }
  }

  // Gets possible intervals for car c to cross the crossing
  intersects(t0[0], tf[0], c);
  for(int i=1; i <= car::accel; i++) {
    c.speed = speed0 + i;
    intersects(t0[2*i-1], tf[2*i-1], c);
    c.speed = speed0 - i;
    intersects(t0[2*i], tf[2*i], c);
  }
  float min_t0 = *(t0.end()-2), max_tf = *(tf.end()-1);

  // Gets the possible collisions
  for(auto& c: array_c_not_belongs) {
    float t00 = t0s.find(c.second.id)->second;
    float tff = tfs.find(c.second.id)->second;

    if(t00 > max_tf || tff < min_t0 || tff == INF) continue;

    t0_col.push_back(t00);
    tf_col.push_back(tff);
  }

  // In case no other car to hit, keep
  if(t0_col.size() == 0) { 
    ac.push_back(std::make_pair(c.id, car::KEEP));
    return;
  }

  // Checks if there is a solution without crash for the problem
  for(int i=0; i < (int)t0.size(); i++) {
    if(tf[i] == INF) continue;
    for(int j=0; j < (int)t0_col.size(); j++) {
      if(are_intervals_disjoint(t0[i], tf[i], t0_col[j], tf_col[j])) {
        // Updates car intervals
        t0s.find(c.id)->second = t0[i];
        tfs.find(c.id)->second = tf[i];

        // Updates car speed
        c.speed = speed0 + ((i%2) ? ((i+1)/2) : -((i+1)/2));

        // Update car values
        array_c_belongs.find(c.id)->second = c;

        // returns action
        if(c.speed > speed0) ac.push_back(std::make_pair(c.id, car::ACCEL));
        else if(c.speed < speed0) ac.push_back(std::make_pair(c.id, car::BREAK));
        else ac.push_back(std::make_pair(c.id, car::KEEP));

        return;
      }
    }
  }

  // No solution found, try breaking
  ac.push_back(std::make_pair(c.id, car::BREAK)); 

  return;
}

////////////////////////////////////////////////////////////////////////////////

void predictor::intersects(float &t0, float &tf, const car& c)
{
  int cross0, crossf;

  if(c.dir == car::HORIZONTAL) { cross0 = cross_h0; crossf = cross_hf; }
  else { cross0 = cross_v0; crossf = cross_vf; }

  if(c.speed > 0 && c.pos <= crossf) {
    t0 = (float)(cross0 - c.pos) / (float)c.speed;
    tf = (float)(crossf + c.size - c.pos) / (float)c.speed;
  } else {
    t0 = INF-1;
    tf = INF;
  }
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

  for(auto& car: cars_h) {
    assert(car.second.pos < max_h);
    map2D[cross_v0*scale_v+1][car.second.pos*scale_h] = '#';
  }
  for(auto& car: cars_v) {
    assert(car.second.pos < max_v);
    map2D[car.second.pos*scale_v][cross_h0*scale_h+1] = '#';
  }
 
#ifdef NDEBUG
  std::cout << "\033[1;1H" << std::endl;
#endif
  for(auto& line: map2D) {
    for(auto& elem: line) {
      std::cout << elem;
    } 
    std::cout << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
