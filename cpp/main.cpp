#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <regex>

#include "LogFile.hpp"
#include "Stat.hpp"
#include "Trace.hpp"
#include "Opt.hpp"


//#include <inttypes.h>
//#ifndef UINT64_MAX
//typedef long long unsigned uint64_t;
//#endif

// Structure all of it in classes?

//std::vector<std::pair<std::string, std::map<std::string, unsigned long long>>>

using StatValueAndOccurrenceMap = std::map<std::string, unsigned long long>;

using StatNameAndValuesVector =
  std::vector<std::pair<std::string, StatValueAndOccurrenceMap>>;


//outOpt="/dev/null"
//If parameter provided then go to the file (or stream?)
// So, we need to have a global TraceEcho object what would create a file?
// And a function called Trace? (free floating function?)
// Define TRACE as globalTrace.Trace()?
// And a use case would be TRACE("The thing to trace?");?


// TODO: Refine the trace_echo from Bash
// Sample of running trace_echo -> probably needs some refinement...
//4 -> should be 2!
//main()->main()->calcTotalShare()->16 - This format is a bit dumb

// In C++ that would need demangling?
// https://stackoverflow.com/questions/733056/is-there-a-way-to-get-function-name-inside-a-c-function
// In C++20 std::source_location::function_name
// For the earlier standards - need to rely on compiler-specific solutions

// TRACE
//


extern Trace globalTrace;
std::vector<Stat> stats; //Get opts...
std::string ifs;
std::string valueFormat;


unsigned long long calcTotalShare(std::string statName)
{
  ENTER_FUNCTION
  // If it matches the specific regex (if there are hits?) then increment
  // the counter...
  globalTrace.Print("gotcha! in the calcTotalShare...");
  LEAVE_FUNCTION
  return 0;
}


// Something like getOpts...
// For now settle for globals...

//# Match STAT(at least one space before value and possibly non-alphanumerical values)VALUE
//ifsOpt="[[:space:]]*[^a-zA-Z0-9]*[[:space:]]+"
//
//# Match VALUE - the default format for value for a given stat
//valueFormatOpt="[0-9]+"

int main(int argc, const char* argv[])
{
  ENTER_FUNCTION

  Opt programOptions(argc, argv);

  //globalTrace.EnterFunction();
  globalTrace.Print("gotcha!in main first");

  calcTotalShare("dummy");

  globalTrace.Print("gotcha! in main second");
  LEAVE_FUNCTION
  return 0;
}

