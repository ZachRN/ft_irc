#include <iostream>

// Trims spaces and tabs
std::string trim_st(std::string str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return (str.substr(first, last + 1));
}

// Trims all whitespace
std::string trim_whitespace(std::string str)
{
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	if (first == last)
		return ("");
	return (str.substr(first, last + 1));
}
