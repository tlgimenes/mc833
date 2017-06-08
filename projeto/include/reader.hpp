#ifndef READER_HPP
#define READER_HPP

#include <string>

class reader
{
  public:
    virtual std::string read(int fd);
};

class tcp_reader: public reader
{
  public:
    std::string read(int fd);
};

class udp_reader: public reader
{
  public:
    std::string read(int fd);
};

#endif

