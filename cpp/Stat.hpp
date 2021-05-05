#ifndef STAT_HPP
#define STAT_HPP


#include <string>
#include <map>
#include <utility>


class Stat
{
public:
  Stat(const std::string& statName);

  std::string GetName() const;

  operator std::string() const;

  bool operator==(const std::string&);

  void IncrementOccurr(const std::string& valueOfStat);

  double CalculateShare(const std::string& valueOfStat);

  void CalculateShare();

  void PrintShare(const std::string& valueOfStat);

  void PrintShare();

private:
  double CalculatePercentage(double part, double total); //TODO: template (take any type, cast to the desired type?)

  std::string name_m;
  unsigned long long totalOccurrences;
  std::map<std::string, unsigned long long> valueOccurrMap;
  std::map<std::string, double> shareOfOccurences;
};


bool operator==(const Stat& stat, const std::string& string);


#endif // #ifndef STAT_HPP
