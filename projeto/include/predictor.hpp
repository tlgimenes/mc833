#ifndef PREDICTOR_HPP 
#define PREDICTOR_HPP 

#include "car.hpp"

#include <vector>

class predictor 
{
  public:
    std::vector<car> cars;

    void add_car(const car& c);

    action update(const car& c) const;

    void exists(const car& c) const;
};

#endif

