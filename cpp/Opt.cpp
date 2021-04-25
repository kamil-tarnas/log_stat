#include "Opt.hpp"


#include <iostream>


Opt::Opt(const int& argc, const char** argv)
{
  // Omit first parameter, which is the name of compiled program
  for (int i = 1; i < argc; ++i)
  {
    tokens.push_back(std::string(argv[i]));
  }
}


Opt::~Opt()
{
}
