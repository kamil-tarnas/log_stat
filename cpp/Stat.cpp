#include "Stat.hpp"
#include "ProgramState.hpp"


#include <iostream>
#include <algorithm>


Stat::Stat(const std::string& statName):
  name_m(statName),
  totalOccurrences(0)
{
}


std::string Stat::GetName() const
{
  return name_m;
}


Stat::operator std::string() const
{
  return name_m;
}


bool Stat::operator==(const std::string& statName)
{
  return name_m == statName;
}


bool operator==(const Stat& stat, const std::string& string)
{
  return stat.GetName() == string;
}


void Stat::IncrementOccurr(const std::string& valueOfStat)
{
  // Use [], if the valueOfStat element does not exist it will be
  // inserted into the map
  valueOccurrMap[valueOfStat]++; //Meh, need to insert the value...
  totalOccurrences++;
}


double Stat::CalculateShare(const std::string& valueOfStat)
{
  auto valueOccurrPairIter = valueOccurrMap.find(valueOfStat);

  if (valueOccurrPairIter != valueOccurrMap.end())
  {
    // TODO: What kind of cast? How to insert to the map (the best method)?
    //shareOfOccurences.insert()
    // TODO: Encapsulate and find better way of upscaling...
    double share =
      CalculatePercentage(valueOccurrPairIter->second, totalOccurrences);
    // Use operator[] for inserting into map if the key element
    // does not exist yet
    shareOfOccurences[valueOfStat] = share;
    return share;
  }
  else
  {
    throw "Requested stat name for calculation of share was not found!";
  }
}


void Stat::CalculateShare()
{
  // This is sorted by the key (the .first...)
  // Calculate share for all the existng elements in valueOccurrMap
  for (const auto& valueOccurPair: valueOccurrMap) //TODO: Happens a lot, maybe a functor/lambda for this?
  {
    double share = CalculatePercentage(valueOccurPair.second, totalOccurrences);
    // Use operator[] for inserting into map if the key element
    // does not exist yet
    shareOfOccurences[valueOccurPair.first] = share;
  }
}


// TODO: Custom printer (like in Bash version) needed here
void Stat::PrintShare(const std::string& valueOfStat)
{
  // Print the value of share of a single stat
}


void Stat::PrintShare()
{
  std::cout << "\nResults for "
            << ProgramState::GetInstance().colors.colorGreen
            <<  name_m << ProgramState::GetInstance().colors.colorReset
            << ":\n";


  std::vector<std::pair<std::string, double>> sortedByOccurr;

  for (const auto& valueOccurPair: shareOfOccurences)
  {
    sortedByOccurr.push_back(valueOccurPair);
  }

  using valOccPair = std::pair<std::string, double>;

  std::sort(sortedByOccurr.begin(),
            sortedByOccurr.end(),
            [](valOccPair a, valOccPair b) {return a.second > b.second;});

  for (const auto& valueOccurPair: sortedByOccurr)
  {
    std::cout << "Share of occurrences of " << name_m
              << " with value " << ProgramState::GetInstance().colors.colorBlue
              << valueOccurPair.first
              << ProgramState::GetInstance().colors.colorReset << ": "
              << ProgramState::GetInstance().colors.colorGreen
              << valueOccurPair.second
              << "%\n" << ProgramState::GetInstance().colors.colorReset;
  }
}


// TODO: What exacly type of casting is done when passing to functions parameters?
double Stat::CalculatePercentage(double part, double total)
{
  return static_cast<double>(part) / static_cast<double>(total) * 100.0;
}
