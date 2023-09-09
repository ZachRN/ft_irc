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

static int validCommand(std::string command)
{
	std::vector<std::string> commands;
	commands.push_back("PASS");
	commands.push_back("NICK");
	commands.push_back("USER");
	commands.push_back("JOIN");
	commands.push_back("PART");
	commands.push_back("PRIVMSG");
	commands.push_back("MODE");
	commands.push_back("INVITE");
	commands.push_back("TOPIC");
	commands.push_back("KICK");
	commands.push_back("PING");
	commands.push_back("PONG");
	commands.push_back("QUIT");
	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); it++)
	{
		if (*it == command)
			return (SUCCESS);
	}
	return (FAILURE);
}

int	input_process(int fd, char buffer[1024], Server *server)
{
	Client* client = server->get_client(fd);
	if (client == nullptr)
		return (NO_CLIENT_FOUND);
	std::string input = buffer;
	std::vector<std::string> parsed = parse_input(input);
	std::vector<std::string>::const_iterator command = parsed.begin();
	if (*command == "\n")
		return (FAILURE);
	int returnvalue = SUCCESS;
	// std::cout << input << std::endl;
	if (validCommand(*command) == FAILURE)
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 421 " + *command + " :Unknown Command\n"));
		return (FAILURE);
	}
	if (parsed.size() < 2)
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " " + *command + " :Not enough parameters\n"));
		return (FAILURE);
	}
	
	//NEED TO SET PASSWORD BEFORE BEING ALLOWED TO CONTINUE
	if (*command == "PASS")
		returnvalue = password(client, parsed);
	if (!client->get_passCorrect())
		return (FAILURE);
	//NICK AND USER MUST BE SET BEFORE BEING ALLOWD TO USE
	if (*command == "NICK")
		returnvalue = nickname(client, parsed, server);
	else if (*command == "USER")
		returnvalue = username(client, parsed);
	if (client->get_verified() == false)
		return (FAILURE);
	//POST BEING VERIFIED AND PASSWORD ENABLED YOU ARE ABLE TO USE ALL MESSAGES
	if (*command == "JOIN")
		returnvalue = join(client, parsed, server);
	else if (*command == "PART")
		returnvalue = leave(client, parsed, server);
	else if (*command == "PRIVMSG")
		returnvalue = privmsg(client, parsed, server, input);
	else if (*command == "MODE")
		returnvalue = mode(client, parsed, server);
	else if (*command == "INVITE")
		returnvalue = invite(client, parsed, server);
	else if (*command == "TOPIC")
		returnvalue = topic(client, parsed, server, input);
	else if (*command == "KICK")
		returnvalue = kick(client, parsed, server, input);
	else if (*command == "PING")
		returnvalue = ping(client, parsed, server, input);
	else if (*command == "PONG")
		returnvalue = pong(client);
	else if (*command == "QUIT")
		returnvalue = quit(client, server);
	return (returnvalue);
	// return (0);
}