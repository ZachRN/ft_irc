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

std::vector<std::string> split(const std::string& input, const char* delimiter)
{
	std::vector<std::string> splits;
	std::string token;
	size_t pos = 0;
	size_t prev = 0;

	while ((pos = input.find(delimiter, prev)) != std::string::npos)
	{
		token = input.substr(prev, pos - prev);
		splits.push_back(token);
		prev = pos + 1;
	}
	splits.push_back(input.substr(prev));

	return (splits);
}
