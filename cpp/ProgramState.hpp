#ifndef PROGRAMSTATE_HPP
#define PROGRAMSTATE_HPP


#include "Opt.hpp"


#include <string>
#include <vector>
#include <memory>


// TODO: Inner class, or some dependent type...
// TODO: Strings need to be initialized in the c-tor...
class Colors
{
public:
  void Set();
  void Unset();
  // TODO: Cannot be auto, see:
  // https://stackoverflow.com/questions/26386010/does-a-declaration-using-auto-match-an-extern-declaration-that-uses-a-concrete
  // auto type will be deduced to std::string in C++20 and above
  // auto type will be deduced to char[] // TODO: is it?
  //constexpr auto colorGreen = "\033[32m";
  //constexpr char colorGreen = "\033[32m"; and externing causes linker error

  // In-class member initializers, since it will always use those values
  std::string colorGreen = "\033[32m";
  std::string colorBlue = "\033[34m";
  std::string colorReset = "\033[39m";
};


class ProgramState
{
public:
  static ProgramState& GetInstance();

  ProgramState(const ProgramState& rhs) = delete;

  ProgramState& operator=(const ProgramState& rhs) = delete;

  void SetProgramState(const Opt& programOptions); // Very tailored and custom function

  // TODO: public currently, for easier access
  bool rawOpt;
  std::string outOpt;
  unsigned bucketsOpt;
  std::string valueFormatOpt;
  std::string ifsOpt;
  Colors colors;

private:
  ProgramState();
};


//// TODO: Read whereto include the externs...
//
//// The whole header should be included anyway, so we could declare an extern
//// just here
//extern ProgramState ProgramState::GetInstance();


#endif // #ifndef PROGRAMSTATE_HPP
