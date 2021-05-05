#ifndef ENTRYREGEX_HPP
#define ENTRYREGEX_HPP


#include "Stat.hpp"


#include <regex>
#include <string>


std::string MakeRegexGroup(std::string stringToWrap);


class EntryRegex
{
public:
  EntryRegex(const std::vector<Stat>& stats,
             const std::string& ifs,
             const std::string& valueFormat);

  ~EntryRegex();

  std::smatch GetMatches(const std::string& stringToSearch) const;

  std::string GetMatch(unsigned matchNumber) const;

  std::size_t GetNumOfMatches() const;

private:
  std::regex regexSkeleton;
  mutable std::smatch matches;
  mutable std::size_t numOfMatches;
};


#endif // #ifndef ENTRYREGEX_HPP
