#include "EntryRegex.hpp"
#include "Trace.hpp"


#include <iostream>


EntryRegex::EntryRegex(const std::vector<Stat>& stats,
                       const std::string& ifs,
                       const std::string& valueFormat)
: numOfMatches(0)
{
  ENTER_FUNCTION

  // Concatenate stat names (process statNames)
  // Concatenating is done for enabling one-pass over a log file

  std::string statNamesRegex;

  for (const auto& stat: stats)
  {
    statNamesRegex += stat.GetName() + "|";
  }
  // Remove the last or - "|"
  statNamesRegex.pop_back();

  std::string stringRegexSkeleton = ("(^\040*)" +
      MakeRegexGroup(statNamesRegex) +
      MakeRegexGroup(ifs) + // default: "[[:space:]]*[^a-zA-Z0-9]*[[:space:]]+"
      MakeRegexGroup(valueFormat)); // default: "[0-9]+"
      //"(.*)"); //TODO: We can heave a tail only if IFS is not an empty string

  regexSkeleton = stringRegexSkeleton;

  globalTrace.Print(stringRegexSkeleton);

  LEAVE_FUNCTION
}


EntryRegex::~EntryRegex()
{
  ENTER_FUNCTION
  LEAVE_FUNCTION
}


// Free function currently
std::string MakeRegexGroup(std::string stringToWrap) //TODO: Move to StatRegex?
{
  ENTER_FUNCTION

  // Wrap the string in parentheses and return
  stringToWrap.append(")");
  stringToWrap.insert(0, "(");
  return stringToWrap;

  LEAVE_FUNCTION
}


std::smatch EntryRegex::GetMatches(const std::string& stringToSearch) const
{
  ENTER_FUNCTION

  std::regex_search(stringToSearch, matches, regexSkeleton);
  numOfMatches = matches.size();

  LEAVE_FUNCTION
  return matches;
}


// If is not valid yet - then call GetMatches...
std::string EntryRegex::GetMatch(unsigned matchNumber) const
{
  ENTER_FUNCTION
  LEAVE_FUNCTION
  return matches.str(matchNumber);
}


std::size_t EntryRegex::GetNumOfMatches() const
{
  ENTER_FUNCTION
  LEAVE_FUNCTION
  return numOfMatches;
}


