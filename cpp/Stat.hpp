#ifndef STAT_HPP
#define STAT_HPP


#include <string>
#include <map>


class Stat
{
public:
	Stat(const std::string& statName);

	std::string GetName();

private:
  std::string name_m;
  std::map<std::string, unsigned long long> valueOccurrMap;
};


#endif // #ifndef STAT_HPP
