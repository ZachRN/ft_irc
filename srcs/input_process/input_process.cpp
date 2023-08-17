#include <iostream>
#include "server.hpp"
#include "input_process.hpp"
#include "utils.hpp"

static std::vector<std::string> parse_input(std::string input)
{
	std::vector<std::string> parsed;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;

	if (!input.empty())
		input = trim_whitespace(input);
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
	Client* client = server->get_client(fd);
	if (client == nullptr)
		return (NO_CLIENT_FOUND);
	std::string input = buffer;
	std::vector<std::string> parsed = parse_input(input);
	std::vector<std::string>::const_iterator command = parsed.begin();
	int returnvalue = SUCCESS;
	std::cout << input << std::endl;
	if (parsed.size() == 0)
		return (FAILURE);
	if (*command == "NICK")
		returnvalue = nickname(client, parsed, server);
	else if (*command == "USER")
		returnvalue = username(client, parsed);
	if (client->get_verified() == false)
		return (FAILURE);
	if (*command == "JOIN")
		returnvalue = join(client, parsed, server);
	else if (*command == "PART")
		returnvalue = leave(client, parsed, server);
	else if (*command == "PRIVMSG")
		returnvalue = privmsg(client, parsed, server, input);
	else if (*command == "MODE")
		returnvalue = mode(client, parsed, server);
	else if (*command == "PING")
		returnvalue = ping(client, parsed, server, input);
	else if (*command == "PONG")
		returnvalue = pong(client);
	else if (*command == "QUIT")
		returnvalue = quit(client, server);
	return (returnvalue);
	// return (0);
}