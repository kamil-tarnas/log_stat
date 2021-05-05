#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <regex>
#include <numeric>


#include "LogFile.hpp"
#include "Stat.hpp"
#include "Trace.hpp"
#include "Opt.hpp"
#include "ProgramState.hpp"


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


unsigned long long mockTestTrace(std::string statName)
{
  ENTER_FUNCTION

  globalTrace.Print("gotcha! in the mockTestTrace()...");

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
  ENTER_FUNCTION // TODO: globalTrace.EnterFunction();

  Opt programOptions(argc, argv);

  // TODO: This would firstly call the default ctor and then just
  // reassign some values, get rid of this double initialization
  // and make the ProgramState instantinable only by passing the Opt object
  ProgramState::GetInstance().SetProgramState(programOptions);

  std::vector<Stat> stats = programOptions.GetStats();

  const int logFileOptPos = 0;
  LogFile logFile(programOptions.GetOpt(logFileOptPos));

  logFile.FindAndProcessStats(stats);

  // Add this point we are ready to calculate and print the results
  for (auto& stat: stats)
  {
    stat.CalculateShare();
    stat.PrintShare();
  }

  std::cout << "\n";


  // TODO: Test and debug...

  globalTrace.Print("gotcha! In main() first");

  mockTestTrace("dummy");

  globalTrace.Print("gotcha! In main() second");

  LEAVE_FUNCTION // TODO: globalTrace.LeaveFunction();
  return 0;
}

