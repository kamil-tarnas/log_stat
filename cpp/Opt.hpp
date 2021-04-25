#ifndef OPT_HPP
#define OPT_HPP


#include <string>
#include <vector>


class Opt
{
public:
  Opt(const int& argc, const char** argv);
  ~Opt();

private:
  std::vector<std::string> tokens;
};


#endif // #ifndef OPT_HPP
