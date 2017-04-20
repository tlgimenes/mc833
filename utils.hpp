///////////////////////////////////////////////////////////////////////////////
//
//  authors: Tiago Lobato Gimenes (118827)
//           Ingrato
//
///////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <string>

///////////////////////////////////////////////////////////////////////////////

enum log_level_t {
  DEBUG,
  WARNING,
  FAIL,
  INFO
};

///////////////////////////////////////////////////////////////////////////////

class log {
  public:
    static log_level_t _log_level;

    static void write(log_level_t level=DEBUG, const std::string& msg) {
      std::string final_msg;

      if(level > log::_log_level) {
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

    static log_level_t& level() { return _log_level; }
};

///////////////////////////////////////////////////////////////////////////////
