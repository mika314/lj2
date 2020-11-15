#pragma once
#include <sstream>

namespace Internal
{
  template <typename... Args>
  void log(const char *fileName, int line, Args &&... args)
  {
    std::ostringstream strm;
    ((strm << args << " "), ...);
    strm << fileName << ":" << line;
    dumpLog(strm.str().c_str());
  }
  template <typename... Args>
  void logErr(const char *fileName, int line, Args &&... args)
  {
    std::ostringstream strm;
    ((strm << args << " "), ...);
    strm << fileName << ":" << line;
    dumpLogErr(strm.str().c_str());
  }
  void dumpLog(const char *);
  void dumpLogErr(const char *);
} // namespace Internal

#define LOG(...) ::Internal::log(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...) ::Internal::logErr(__FILE__, __LINE__, __VA_ARGS__)
