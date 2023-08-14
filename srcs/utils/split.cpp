#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string& input, char delimiter)
{
	std::vector<std::string> splits;
	std::stringstream str_stream(input);
	std::string token;

	while (std::getline(str_stream, token, delimiter))
		splits.push_back(token);

	return (splits);
}
