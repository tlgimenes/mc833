////////////////////////////////////////////////////////////////////////////////
/**
 * @file test_predictor.cpp
 * @date 2017-06-08
 * @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
 *
 * @copyright Tiago Lobato Gimenes 2017. All rights reserved.
 *
 */
////////////////////////////////////////////////////////////////////////////////

#include "car.hpp"
#include "client_car.hpp"
#include "predictor.hpp"
#include "utils.hpp"

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

#define test(exp) \
  if(!(exp)) { log::write(FAIL, #exp); }

////////////////////////////////////////////////////////////////////////////////

void test2()
{
  log::write(INFO, "Starting test2 - Test add_car and exists ... ");

  car c1(0, 2, car::HORIZONTAL), c2(0, 1, car::VERTICAL);
  predictor pred(10, 12, 8, 9);
  
  pred.add_car(c1);

  test(pred.exists(c1) == true);
  test(pred.exists(c2) == false);
  
  pred.add_car(c2);
  
  test(pred.exists(c2) == true);
  
  log::write(INFO, "pass");
}

////////////////////////////////////////////////////////////////////////////////

void test1()
{
  log::write(INFO, "Starting test1 - Test car constructor ... ");

  predictor pred(10, 12, 8, 9);
  
  car c1(0, 2, car::HORIZONTAL), c2(0, 1, car::VERTICAL);
  
  test(c1.id != c2.id);
  test(c1.pos == c2.pos);
  test(c1.speed != c2.speed);
  test(c1.dir != c2.dir);
  
  log::write(INFO, "pass");
}

////////////////////////////////////////////////////////////////////////////////

void test0()
{
  log::write(INFO, "Starting test0 - Test predictor constructor ... ");
  
  predictor pred(10, 12, 8, 9);
  
  test(pred.cars.size() == 0);
  
  log::write(INFO, "pass");
}

////////////////////////////////////////////////////////////////////////////////

void test3()
{
  log::write(INFO, "Starting test3 - Test scene drawer  ... ");
  
  predictor pred(50, 60, 60, 74);

  pred.add_car(car(80, 2, car::HORIZONTAL));
  pred.add_car(car(40, 2, car::HORIZONTAL));
  pred.add_car(car(20, 2, car::HORIZONTAL));
  pred.add_car(car(20, 1, car::VERTICAL));
  pred.add_car(car(40, 1, car::VERTICAL));
  pred.add_car(car(70, 1, car::VERTICAL));

  pred.draw();
  
  log::write(INFO, "pass");
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) 
{
  log::log_level = DEBUG;
  log::write(INFO, "Starting Predictor Tests");

  test0();
  test1();
  test2();
  test3();

  return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
