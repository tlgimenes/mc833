////////////////////////////////////////////////////////////////////////////////
/**
 * @file client_car.hpp
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

#ifndef CLIENT_CAR_HPP
#define CLIENT_CAR_HPP

////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <ratio>  
#include "car.hpp"

////////////////////////////////////////////////////////////////////////////////

class client_car: public car
{
  public:
    // Constructor
    client_car(int pos, int speed, direction dir, int size=1);

    void server_update(action ac);  // Update based on action
    void self_update();             // Update own position

  private:
    double delta_pos = 0;  // Decimal part of position
    std::chrono::high_resolution_clock::time_point last_update; // When was the last update
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! CLIENT_CAR_HPP */

////////////////////////////////////////////////////////////////////////////////
