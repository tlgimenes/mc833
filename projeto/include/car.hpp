////////////////////////////////////////////////////////////////////////////////
/**
 * @file car.hpp
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

#ifndef CAR_HPP
#define CAR_HPP

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

class car
{
  public:
    // Action for car to take
    enum action {ACCEL, BREAK, KEEP, AMBULANCE}; 
    // Car direction
    enum direction {HORIZONTAL, VERTICAL};

    // Max acceleration between ticks
    static const int accel=1;
    // Invalid car id
    static const int invalid_id;
    // Number of instances of this class
    static int ncars;
  
    // Constructors
    car(const car& c);
    car(int pos, int speed, direction dir, int size=1);
    car(const std::string& str);

    direction dir; // Car direction
    int pos;       // Car position
    int speed;     // Car speed
    int size;      // Car size
    int id;        // Car unique id

    std::string to_string(); // Class to string
    
    car& operator=(const car& c);
    bool operator==(const car& c);
};

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

#endif /*! CAR_HPP */

////////////////////////////////////////////////////////////////////////////////
