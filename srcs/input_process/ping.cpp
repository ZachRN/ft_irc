#include <iostream>
#include "client.hpp"

#include "server.hpp"

int ping(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	size_t	colon_pos = unparsed.find_first_of(":");
	std::string user_msg = "";
	if (colon_pos != std::string::npos)
		user_msg = unparsed.substr(colon_pos + 1) + "\n";
	else if (parsed_input.size() > 1)
		user_msg = parsed_input.at(1);
	server->send_msg(client->get_fd(),
			":" + server->get_config().get_host() +
			" PONG " + server->get_config().get_serverName() +
			" :" + user_msg + "\r\n");
	return (SUCCESS);
}
