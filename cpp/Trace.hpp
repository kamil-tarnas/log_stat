#ifndef TRACE_HPP
#define TRACE_HPP


#include <string>
#include <fstream>


// Need to remove "()->" and use "(): " in the last function... Yeah, this is the way to do it...
//globalTrace.stackTrace_m.erase(globalTrace.stackTrace_m.end() - Trace::callChainSuffix.length(), globalTrace.stackTrace_m.end())

#define ENTER_FUNCTION \
    {globalTrace.numOfFunctionsOnStack_m++; \
    globalTrace.stackTrace_m += std::string(__func__) + "()->";} \

#define LEAVE_FUNCTION \
    {globalTrace.numOfFunctionsOnStack_m--; \
    globalTrace.stackTrace_m. \
    erase(globalTrace.stackTrace_m.length() - Trace::callChainSuffixLen, std::string::npos); \
    globalTrace.stackTrace_m. \
    erase(globalTrace.stackTrace_m.rfind(Trace::callChainSuffix) + Trace::callChainSuffixLen, std::string::npos);} \


// TODO: Redefine this with a singleton friend class having access to Trace global
// trace object, but the long-term goal is to have a separate tracer that could be
// used...


// Maybe use this - might be a good idea...
//https://stackoverflow.com/questions/353180/how-do-i-find-the-name-of-the-calling-function
class Trace
{
public:
  // auto type will be deduced to std::string in C++20 and above
  // auto type will be deduced to char[] // TODO: is it?
  static constexpr auto callChainSuffix = "()->";
  static constexpr auto callChainSuffixLen = 4;
  static constexpr auto callEndSuffix = "(): ";

  Trace(const std::string& fileName);

  ~Trace();

  void EnterFunction();

  void LeaveFunction();

  void Print(const std::string& text);

  //private:
public: // TODO: current hack, will be changed with trace_echo C++ implementation
  std::ofstream traceStream_m;
  unsigned numOfFunctionsOnStack_m;
  std::string stackTrace_m;
};


#endif // #ifndef TRACE_HPP
