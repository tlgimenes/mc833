#ifndef CLIENT_CAR_HPP
#define CLIENT_CAR_HPP

#include "car.hpp"

class client_car: public car
{
  public:

    void server_update(enum action);

    void self_update();
};

#endif

