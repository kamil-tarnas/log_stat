#include "Opt.hpp"
#include "Stat.hpp"


#include <iostream>
#include <vector>


Opt::Opt(const int& argc, const char** argv)
{
  // Omit first parameter, which is the name of compiled program
  for (int i = 1; i < argc; ++i)
  {
    tokens.push_back(std::string(argv[i]));
  }

  // TODO: Move that, so it will be easier to reuse...
  flags = GetFlags();
}


Opt::~Opt()
{
}


// TODO: More like some function called GetStats() should operate ON A PROGRAM
//       options (Opt object)
std::vector<Stat> Opt::GetStats() const
{
  // TODO: Explore the RVO for returning std::vector
  std::vector<Stat> stats;
	// TODO: What to do with the stat names? Remove globals?
	// Process program options to extract the stats

  for (auto tokenIter = tokens.begin() + 1;
       tokenIter < tokens.end();
       ++tokenIter)
  {
    if (tokenIter->front() != '-')
    {
      stats.push_back(Stat(*tokenIter));
    }
  }

  return stats;
}


std::vector<std::string> Opt::GetFlags() const
{
  std::vector<std::string> flags;

  for (auto tokenIter = tokens.begin() + 1;
       tokenIter < tokens.end();
       ++tokenIter)
  {
    // Return only the things matching "-" or "--"
    // Basically anything that starts with "-" should be consider
    if (tokenIter->front() == '-')
    {
      flags.push_back(*tokenIter);
    }
  }
  // Return cached object if it was processed already
  return flags;
}


// Gets the program option token, by default the zeroth one
std::string Opt::GetOpt(int number)
{
  return tokens.at(number);
}


// Need to cover both --flag and -flag and should cover --flag== and -flag= !!!
bool Opt::IsFlagPresent(const std::string& searchedFlag) const
{
  // TODO: with the new definition it can be --raw or -r, so it is good?
  for (const auto& flag: flags) // TODO: Keeping it in a set would probably be a better option
    //- a set with custom hash function - maybe half "-" and the second half
    //"--" and then go with fist letter matching... Make it continous memory also...
  {
    auto positionOfFlagName = flag.find_first_not_of('-');
    auto positionOfSearchedFlagName = searchedFlag.find_first_not_of('-');

    // TODO: Remove the magic number, parametrize the Opt...
    if (positionOfFlagName <= 2 && positionOfSearchedFlagName <= 2)
    {
      if (std::string(flag, positionOfFlagName, std::string::npos) ==
          std::string(searchedFlag, positionOfSearchedFlagName, std::string::npos))
      {
        return true;
      }
    }
  }
  return false;
}


std::string Opt::GetOptionValue(const std::string& searchedOption) const
{
  for (const auto& flag: flags)
  {
    auto positionOfFlagName = flag.find_first_not_of('-');
    auto positionOfSearchedOptionName = searchedOption.find_first_not_of('-');

    // TODO: Remove the magic number, parametrize the Opt...
    if (positionOfFlagName <= 2 && positionOfSearchedOptionName <= 2)
    {
      auto positionOfEqualChar = flag.find_first_of('=');

      if (positionOfEqualChar != std::string::npos)
      {
        if (std::string(flag, positionOfFlagName, positionOfEqualChar - 2) ==
            std::string(searchedOption, positionOfSearchedOptionName, std::string::npos))
        {
          return std::string(flag, positionOfEqualChar + 1, std::string::npos);
        }
      }
    }
  }
  return std::string();
}

//std::string Opt::GetOptionValue(const std::string& searchedOption) const
//{
//  for (const auto& flag: flags)
//  {
//    auto positionOfFlagName = flag.find_first_not_of('-');
//    auto positionOfSearchedOptionName = searchedOption.find_first_not_of('-');
//
//    std::cout << flag << " " << positionOfFlagName << " " <<  positionOfSearchedOptionName << " ";
//
//    // TODO: Remove the magic number, parametrize the Opt...
//    if (positionOfFlagName <= 2 && positionOfSearchedOptionName <= 2)
//    {
//      auto positionOfEqualChar = flag.find_first_of('=');
//
//      //std::cout << positionOfEqualChar << "\n";
//
//      if (positionOfEqualChar != std::string::npos)
//      {
//        std::cout << std::string(flag, positionOfFlagName, positionOfEqualChar - 2) << "\n";
//        std::cout << std::string(searchedOption, positionOfSearchedOptionName, std::string::npos) << "\n";
//
//        if (std::string(flag, positionOfFlagName, positionOfEqualChar - 2) ==
//            std::string(searchedOption, positionOfSearchedOptionName, std::string::npos))
//        {
//          std::cout << "RETURNS: "<< std::string(flag, positionOfEqualChar + 1, std::string::npos) << "\n\n";
//          return std::string(flag, positionOfEqualChar + 1, std::string::npos);
//        }
//      }
//    }
//  }
//  return std::string();
//}

