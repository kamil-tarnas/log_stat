#include "ProgramState.hpp"
#include "Opt.hpp"
#include "Trace.hpp"


#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <iomanip>


ProgramState& ProgramState::GetInstance()
{
  static ProgramState globalState;
  return globalState;
}


// Write the default values, that could be changed by flags after calling main()
ProgramState::ProgramState() :
  rawOpt(false),
  outOpt("/dev/null"),
  bucketsOpt(4),
  valueFormatOpt("[0-9]+"),
  ifsOpt("\040*[^a-zA-Z0-9]*\040*")
{
  // Configure cout object for printing
  std::cout << std::fixed;
  std::cout << std::setprecision(3);
}


// TODO: Having the flags in set would be better?
void ProgramState::SetProgramState(const Opt& programOptions) // Pass the vector of string flags...
{
  // TODO: Wrap it in something like "Check flags"

  // Commonly used
  std::string longOpt;
  std::string shortOpt;


  // --raw -r
  if (programOptions.IsFlagPresent("raw") ||
      programOptions.IsFlagPresent("r"))
  {
    rawOpt = true;
    colors.Unset();
  }

  // --raw=* -r=*
  longOpt = programOptions.GetOptionValue("raw");
  shortOpt = programOptions.GetOptionValue("r");

  if (!longOpt.empty())
  {
    if (longOpt == "true")
    {
      rawOpt = true;
      colors.Unset();
    }
  }
  else if (!shortOpt.empty())
  {
    if (shortOpt == "false")
    {
      rawOpt = false;
    }
  }


  // --out -o
  if (programOptions.IsFlagPresent("out") ||
      programOptions.IsFlagPresent("o"))
  {
    outOpt = "log";
  }

  // --out=* -o=*
  longOpt = programOptions.GetOptionValue("out");
  shortOpt = programOptions.GetOptionValue("o");

  if (!longOpt.empty())
  {
    outOpt = longOpt;
  }
  else if (!shortOpt.empty())
  {
    outOpt = shortOpt;
  }

  // --buckets -b
  if (programOptions.IsFlagPresent("buckets") ||
      programOptions.IsFlagPresent("b"))
  {
    bucketsOpt = 4;
  }

  // --buckets=* -b=*
  longOpt = programOptions.GetOptionValue("buckets");
  shortOpt = programOptions.GetOptionValue("b");

  if (!longOpt.empty())
  {
    bucketsOpt = std::stoul(longOpt);
  }
  else if (!shortOpt.empty())
  {
    bucketsOpt = std::stoul(shortOpt);
  }


  // --ifs -i
  // Just ignore and go ahead with constructor values

  // --ifs=* -i=*
  longOpt = programOptions.GetOptionValue("ifs");
  shortOpt = programOptions.GetOptionValue("i");

  if (!longOpt.empty())
  {
    ifsOpt = longOpt;
  }
  else if (!shortOpt.empty())
  {
    ifsOpt = shortOpt;
  }


  // --value-format -v
  // Just ignore and go ahead with constructor values

  // ---value-format=* -v=*
  longOpt = programOptions.GetOptionValue("value-format");
  shortOpt = programOptions.GetOptionValue("v");

  if (!longOpt.empty())
  {
    valueFormatOpt = longOpt;
  }
  else if (!shortOpt.empty())
  {
    valueFormatOpt = shortOpt;
  }

  // TODO: Just delegate to Opt to get the value of falg or option and come back
  // and do the processing here... Get the string or? Actually we have that already...
}


void Colors::Set()
{
  colorGreen = "\033[32m";
  colorBlue = "\033[34m";
  colorReset = "\033[39m";
}


void Colors::Unset()
{
  colorGreen.clear();
  colorBlue.clear();
  colorReset.clear();
}
