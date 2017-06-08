#ifndef CAR_HPP
#define CAR_HPP

class car
{
  public:
    car(const std::string& str);
    car(const car& c);
    car(int pos, int speed);

    enum action {ACCEL, BREAK, KEEP, AMBULANCE}; 
    enum direction {HORIZONTAL, VERTICAL};

    int pos;
    int speed;

    int id;

    static const int accel=1;

    std::string to_string();
};

#endif

