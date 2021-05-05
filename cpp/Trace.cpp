#include "Trace.hpp"

#include <string>
#include <fstream>


// Create the global trace object
Trace globalTrace("/dev/null");


// TODO: Redefine this with a singleton friend class having access to Trace global
// trace object, but the long-term goal is to have a separate tracer that could be
// used...


Trace::Trace(const std::string& fileName) :
  traceStream_m(fileName),
  numOfFunctionsOnStack_m(0), // TODO: Could be incorrect? Depends on where is the object created
  // we could throw if it is NOT created before main()
  stackTrace_m("") // XD for now
{
  // Empty currently
}


Trace::~Trace()
{
  traceStream_m.close();
}


void Trace::EnterFunction() // TODO: Needs to be aligned with ENTER_FUNCTION for now...
{
  numOfFunctionsOnStack_m++;
  stackTrace_m += std::string(__func__) + "()->"; // This does not take the function called!
}


void Trace::LeaveFunction() // TODO: Needs to be aligned with LEAVE_FUNCTION for now...
{
  numOfFunctionsOnStack_m--;

  // Remove the "()->" string from the end
  stackTrace_m.
  erase(stackTrace_m.length() - callChainSuffixLen, std::string::npos);

  // Remove (starting form the end) everything till first "()->"
  stackTrace_m.
  erase(stackTrace_m.rfind(callChainSuffix) + callChainSuffixLen, std::string::npos);
}


void Trace::Print(const std::string& text)
{
  std::string textToPrint = stackTrace_m;

  textToPrint.erase(textToPrint.end() -
                    callChainSuffixLen, textToPrint.end());

  textToPrint += callEndSuffix;

  traceStream_m << "depth=" << numOfFunctionsOnStack_m << " "
      << textToPrint << text << "\n";
}


void Trace::ChangeStream(const std::string& stream)
{
  traceStream_m.close();
  traceStream_m.open(stream);
}
