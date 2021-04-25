#include "Stat.hpp"


Stat::Stat(const std::string& statName):
  name_m(statName)
{
}

std::string Stat::GetName()
{
	return name_m;
}
