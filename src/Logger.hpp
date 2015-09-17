#ifndef PARFAIT_LOGGER_HPP
#define PARFAIT_LOGGER_HPP

Parfait::Logger* Parfait::Logger::logger = nullptr;
FILE* Parfait::Logger::file = nullptr;

namespace Parfait {

  inline Logger::Logger() {
      openFile("log.txt");
  }
  inline Logger::~Logger() {
      if(logger != nullptr)
          delete logger;
  }
  inline Logger* Logger::instance() {
      if (logger != nullptr)
          return logger;
      logger = new Logger();
      return logger;
  }
  inline void Logger::log(const char *format, ...) {
      auto output = instance();
      char buffer[256];
      va_list args;
      va_start (args, format);
      vsprintf(buffer, format, args);
      va_end (args);
      fprintf(output->file, "%s", buffer);
  }
  inline void Logger::openFile(std::string file_name) {
      file = fopen(file_name.c_str(), "w");
      if(file == NULL)
          throw std::logic_error("Could not open log file <" + file_name + ">");
  }
  inline void Logger::setFileName(std::string file_name) {
      auto i = instance();
      fclose(i->file);
      openFile(file_name);
  }
  inline void Logger::setOutputStream(FILE *output) {
      auto i = instance();
      fclose(i->file);
      i->file = output;
  }
}

#endif 
