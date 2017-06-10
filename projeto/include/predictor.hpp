////////////////////////////////////////////////////////////////////////////////
/**
 * @file predictor.hpp
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

#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

////////////////////////////////////////////////////////////////////////////////

#include "car.hpp"

#include <map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class predictor 
{
  private:
    int cross_h0, cross_hf; // Horizontal intersection
    int cross_v0, cross_vf; // Vertical intersection

    const int max_h = 1e2;
    const int max_v = 1e2;

    std::map<int, float> t0s, tfs;

  protected:
    void intersects(float &t0, float &tf, const car& c);

  public:
    std::map<int, car> cars_h, cars_v;

    predictor(int cross_h0, int cross_hf, int cross_v0, int cross_vf);

    void add_car(const car& c);
    void remove_car(const car& c);

    car::action update(car& c);

    bool exists(const car& c) const;

    void draw();
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! PREDICTOR_HPP */

////////////////////////////////////////////////////////////////////////////////
