////////////////////////////////////////////////////////////////////////////////
/**
 * @file utils.hpp
 * @date 2017-06-08
 * @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
 * @author Renato Vargas
 *
 * @copyright Tiago Lobato Gimenes 2017. All rights reserved.
 *
 * @brief
 *
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef UTILS_HPP
#define UTILS_HPP

////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////

using log_level_t = enum log_level_t { FAIL, WARNING, INFO, DEBUG };

///////////////////////////////////////////////////////////////////////////////

class log {
  public:
    static log_level_t log_level;

    static void write(log_level_t level=DEBUG, const std::string& msg="Unknown") {
      std::string final_msg;

      if(level <= log::log_level) {
        switch(level) {
          case DEBUG:
            std::cout << "[DEBUG]: " << msg << std::endl;
            break;
          case WARNING:
            std::cout << "[WARNING]: " << msg << std::endl;
            break;
          case INFO:
            std::cout << "[INFO]: " << msg << std::endl;
            break;
          case FAIL: 
            std::cout << "[FAIL]: " << msg << std::endl;
            exit(EXIT_FAILURE);
          default: final_msg = "Unknown error code";
        }
      }
    }

    static log_level_t& level() { return log_level; }
};

///////////////////////////////////////////////////////////////////////////////

log_level_t log::log_level = INFO;

////////////////////////////////////////////////////////////////////////////////

#endif /*! UTILS_HPP */

////////////////////////////////////////////////////////////////////////////////
