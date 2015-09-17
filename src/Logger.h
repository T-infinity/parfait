#ifndef PARFAIT_LOGGER_H
#define PARFAIT_LOGGER_H

#include <string>
#include <stdio.h>
#include <stdarg.h>

namespace Parfait{
  class Logger{
  public:
      static void log(const char *format, ...);
      ~Logger();

      static void setFileName(std::string file_name);
      static void setOutputStream(FILE *output);
  private:
      Logger();
      static Logger* logger;
      static Logger* instance();
      static FILE *file;

      static void openFile(std::string file_name);
  };


}
#include "Logger.hpp"
#endif 
