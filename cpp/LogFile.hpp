#ifndef LOGFILE_HPP
#define LOGFILE_HPP


#include "Stat.hpp"


#include <string>
#include <fstream>
#include <vector>


class LogFile
{
public:
  LogFile(const std::string& fileName);

  ~LogFile();

  LogFile(const LogFile& rhs) = delete;

  LogFile(LogFile&& rhs);

  LogFile& operator=(const LogFile& rhs) = delete;

  LogFile& operator=(LogFile&& rhs);

  // Could be auto return type, but C++14 is needed for that
  std::istream& GetLine(std::string& line);

  void FindAndProcessStats(std::vector<Stat>& stats);

private:
  std::ifstream fileStream_m;
};


#endif // #ifndef LOGFILE_HPP
