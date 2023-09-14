#include <map>
#include "server.hpp"


int username(Client* client, std::vector<std::string> parsed_input)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (client->get_username() == "")
		return (client->set_username((*(++(parsed_input.begin())))));
	//apparently you cant change user/host name but its not unique
	// if (parsed_input.size() > 2)
	// 	return (FAILURE);
	// send_join(client, channel, (*(++(parsed_input.begin()))));
	return (SUCCESS);
}