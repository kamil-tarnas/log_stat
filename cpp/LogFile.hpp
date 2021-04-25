#ifndef LOGFILE_HPP
#define LOGFILE_HPP


#include <string>
#include <fstream>


class LogFile
{
public:
  LogFile(const std::string& fileName);

  ~LogFile();

  // Could be auto return type, but C++14 is needed for that
  std::istream& GetLine(std::string& line);

  int FindAndProcessStats();

private:
  std::ifstream fileStream_m;
};


#endif // #ifndef LOGFILE_HPP
