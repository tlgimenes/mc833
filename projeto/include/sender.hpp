#ifndef SENDER_HPP 
#define SENDER_HPP 

#include <string>

class sender
{
  public:
    virtual void send(std::string& str, int fd);
};

class tcp_sender: public sender
{
    void send(std::string& str, int fd);
};

class udp_sender: public sender
{
    void send(std::string& str, int fd);
};

#endif

