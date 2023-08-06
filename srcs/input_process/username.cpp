#include <map>
#include "server.hpp"
#include "run_server.hpp"

int username(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	(void)server;
	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (client->get_username() == "")
		return (client->set_username((*(++(parsed_input.begin())))));
	//apparently you cant change user/host name but its not unique
	// if (parsed_input.size() > 2)
	// 	return (FAILURE);
	// send_join(client, channel, (*(++(parsed_input.begin()))));
	return (SUCCESS);
}