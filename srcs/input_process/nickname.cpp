#include <map>
#include "server.hpp"
#include "run_server.hpp"

static void send_nick(Client* client, Server* server, std::string old_name)
{
	std::map<int, Client>* clientList = server->get_clientList();
	std::string message = ":" + server->get_config().get_serverName() + " " + old_name + " " + client->get_nickname() + "\n"; 

	for (std::map<int, Client>::iterator to_send = clientList->begin(); to_send != clientList->end(); to_send++)
	{
		send_msg(to_send->second.get_fd(), message);
	}
}

int nickname(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string old_name = client->get_username();
	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (client->get_nickname() == "")
		return (client->set_nickname((*(++(parsed_input.begin())))));
	if (parsed_input.size() > 2)
		return (FAILURE);
	send_nick(client, server, old_name);
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]