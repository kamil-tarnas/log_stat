#include "LogFile.hpp"
#include "Stat.hpp"
#include "EntryRegex.hpp"
#include "Trace.hpp"
#include "ProgramState.hpp"


#include <iostream>
#include <fstream>
#include <regex>
#include <string>


LogFile::LogFile(const std::string& fileName)
{
  ENTER_FUNCTION

  fileStream_m.open(fileName, std::ios::in);

  if (fileStream_m.is_open())
  {
    globalTrace.Print("'" + fileName + "' file opened successfully\n");
  }
  else
  {
    globalTrace.Print("Cannot open " + fileName + "\n");
  }

  LEAVE_FUNCTION
}


LogFile::~LogFile()
{
  ENTER_FUNCTION

  fileStream_m.close();

  LEAVE_FUNCTION
}


LogFile::LogFile(LogFile&& rhs)
{
  fileStream_m = std::move(rhs.fileStream_m);
}


LogFile& LogFile::operator=(LogFile&& rhs)
{
  fileStream_m = std::move(rhs.fileStream_m);
  return *this;
}


// Could be auto return type, but C++14 is needed for that
std::istream& LogFile::GetLine(std::string& line)
{
  ENTER_FUNCTION
  LEAVE_FUNCTION
  return std::getline(fileStream_m, line);
}


void LogFile::FindAndProcessStats(std::vector<Stat>& stats)
{
  ENTER_FUNCTION

  std::string ifs =
    ProgramState::GetInstance().ifsOpt; // default: "\040*[^a-zA-Z0-9]*\040*"

  std::string valueFormat =
    ProgramState::GetInstance().valueFormatOpt; //default: "[0-9]+"

  EntryRegex lineRegex(stats, ifs, valueFormat);

  // TODO: In EntryRegex or in ProgramState::GetInstance()?
  const int statNamePos = 2;
  //const int ifsPos = 3; // TODO: Unused currently
  const int valueFormatPos = 4;

  std::string line;
  std::string info; // TODO: Debug, remove

  while (GetLine(line))
  {
    lineRegex.GetMatches(line);

    // TODO: Do that in the trace_echo...
    info += "The number of matches is " + std::to_string(lineRegex.GetNumOfMatches()) + "\n";
    for (unsigned i = 0; i < lineRegex.GetNumOfMatches(); ++i)
    {
      info += std::to_string(i) + " " + lineRegex.GetMatch(i) + "\n";
      // zeroth match is the whole expression, the latter are the groups
    }

    if (lineRegex.GetNumOfMatches())
    {
      std::string matchedStatName = lineRegex.GetMatch(statNamePos);

      auto statIter = std::find(stats.begin(), stats.end(), matchedStatName);

      if (statIter != stats.end())
      {
        statIter->IncrementOccurr(lineRegex.GetMatch(valueFormatPos));
      }
      else
      {
        throw "Line matched the regex, but stat was not found in the vector!";
      }
    }
  }

  globalTrace.Print(info);

  LEAVE_FUNCTION
}
