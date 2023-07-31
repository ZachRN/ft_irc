#include <iostream>
#include "server.hpp"

static std::vector<std::string> parse_input(std::string input)
{
	std::vector<std::string> parsed;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;

	while ((pos = input.find(delimiter)) != std::string::npos) {
	    token = input.substr(0, pos);
		parsed.push_back(token);
	    input.erase(0, pos + delimiter.length());
	}
	parsed.push_back(input);
	return (parsed);
}

void	print_vector(std::vector<std::string> parsed)
{
	for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); it++)
		std::cout << (*it) << std::endl;
}

int	input_process(int fd, char buffer[1024], Server *server)
{
	std::string input = buffer;
	Client* client = server->get_client(fd);
	(void)client;
	std::vector<std::string> parsed = parse_input(input);
	std::vector<std::string>::const_iterator command = parsed.begin();
	if (*command == "JOIN")
		std::cout << "JOIN" << std::endl;
	// print_vector(parsed);
	// std::cout << "VECTOR DONE" << std::endl;
	return (SUCCESS);
}