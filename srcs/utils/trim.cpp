#include <iostream>

std::string trim(std::string str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return (str.substr(first, last + 1));
}
