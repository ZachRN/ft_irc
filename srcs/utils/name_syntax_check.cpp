#include "iostream"
#include "server.hpp"

bool	name_syntax_check(std::string str)
{
	//This entire function checks the syntax of the str
	//If every character is not between 0-9A-Za-z or is ' ' - _ then return error
	if (str.length() < 1 || str.length() > 15)
		return (FAILURE);
	for (std::size_t i = 0; i < str.length(); i++)
	{
		if (!((str[i] >= 'a' && str[i] <= 'z')
		|| (str[i] >= 'A' && str[i] <= 'Z')
		|| (str[i] >= '0' && str[i] <= '9')
		|| (str[i] == '.' || str[i] == '-' || str[i] == '_')))
		{
			return (FAILURE);
		}
	}
	return (SUCCESS);
}