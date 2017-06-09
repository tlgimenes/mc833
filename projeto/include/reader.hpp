////////////////////////////////////////////////////////////////////////////////
/**
 * @file reader.hpp
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

#ifndef READER_HPP
#define READER_HPP

////////////////////////////////////////////////////////////////////////////////

#include <string>

////////////////////////////////////////////////////////////////////////////////

class reader
{
  public:
    virtual std::string read(int fd);
};

////////////////////////////////////////////////////////////////////////////////

class tcp_reader: public reader
{
  public:
    std::string read(int fd);
};

////////////////////////////////////////////////////////////////////////////////

class udp_reader: public reader
{
  public:
    std::string read(int fd);
};

////////////////////////////////////////////////////////////////////////////////

#endif /*! READER_HPP */

////////////////////////////////////////////////////////////////////////////////
