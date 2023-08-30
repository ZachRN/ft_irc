#ifndef SPLIT_HPP
# define SPLIT_HPP

# include <string>
# include <vector>

std::vector<std::string> split(const std::string& input, char delimiter);
std::vector<std::string> split(const std::string& input, const char* delimiter);

#endif
