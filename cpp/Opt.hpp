#ifndef OPT_HPP
#define OPT_HPP


#include "Stat.hpp"


#include <string>
#include <vector>


class Opt
{
public:
  Opt(const int& argc, const char** argv);

  ~Opt();

  std::vector<Stat> GetStats() const;

  // TODO: Consider the type
  std::vector<std::string> GetFlags() const;

  std::string GetOpt(int number = 0);

  // TODO: Depending on the flag format return true if flag exist?
  // How to indicate that flag exist but does not have a value?
  std::string GetFlagValue() const; //TODO: To be removed with the new definition

  bool IsFlagPresent(const std::string& searchedFlag) const;

  bool IsOptionPresent(const std::string& searchedOption) const;

  std::string GetOptionValue(const std::string& searchedOption) const;

private:
  std::vector<std::string> tokens;
  std::vector<std::string> flags;
};


#endif // #ifndef OPT_HPP
