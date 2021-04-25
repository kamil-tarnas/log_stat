#include "LogFile.hpp"
#include "Stat.hpp"


#include <iostream>
#include <fstream>
#include <regex>
#include <string>


// TODO: Modularize the below need to be removed:
// start
std::string MakeRegexGroup(std::string stringToWrap)
{
  // Wrap the string in parentheses and return
  stringToWrap.append(")");
  stringToWrap.insert(0, "(");
  return stringToWrap;
}

extern std::vector<Stat> stats; //Get opts...
extern std::string ifs;
extern std::string valueFormat;
//end

LogFile::LogFile(const std::string& fileName)
{
  fileStream_m.open(fileName, std::ios::in);

  if (fileStream_m.is_open())
  {
    // Should be equivalence of trace_echo
    std::cout << "'" << fileName << "' file opened successfully\n";
  }
  else
  {
    std::cout << "Cannot open "<< fileName << "\n";
  }
}


LogFile::~LogFile()
{
  fileStream_m.close();
}


// Could be auto return type, but C++14 is needed for that
std::istream& LogFile::GetLine(std::string& line)
{
  return std::getline(fileStream_m, line);
}


// Pass heer the opts? More modular would be to pass the stats separately
int LogFile::FindAndProcessStats()
{
  // TODO: Concatenate stat names (process statNames)
  // Concatenating is done for enabling one-pass over a log file
  std::string statNamesRegex;

  for (auto& stat: stats)
  {
    statNamesRegex += stat.GetName() + "|";
  }
  // Make regex group

  // Wrap it in regex groups...
  //Yes, and make this a separate class like EntrySkeleton
  std::regex
    statSkeletonEntry("(^\040*)"+
                      MakeRegexGroup(statNamesRegex) +
                      MakeRegexGroup(ifs) +
                      MakeRegexGroup(valueFormat) +
                      "(.*)");

  // TODO: Check if that is even corect...
  const int statNamePos = 1;
  const int ifsPos = 2;
  const int valueFormatPos = 3;

  std::string line;

  while (this->GetLine(line))
  {
    std::smatch matches;
    std::regex_search(line, matches, statSkeletonEntry);

    if (matches.size()) // TODO: Check if that is actually correct
    {
      // Check what has been matched in the statName group and process...
    }

  //stats.push_back()
  }

  return 0;
}
