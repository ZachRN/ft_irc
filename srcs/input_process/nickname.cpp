#include <map>
#include "server.hpp"
#include "run_server.hpp"

static void send_nick(Client* client, Server* server, std::string old_name)
{
	std::map<int, Client>* clientList = server->get_clientList();
	std::string message = ":" + old_name + " NICK " + client->get_nickname() + "\n"; 
	
	std::cout << message << std::endl;
	for (std::map<int, Client>::iterator to_send = clientList->begin(); to_send != clientList->end(); to_send++)
	{
		server->send_msg(to_send->second.get_fd(), message);
	}
}

int nickname(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string old_name = client->get_nickname();
	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (client->get_nickname() == "")
		return (client->set_nickname((*(++(parsed_input.begin())))));
	if (parsed_input.size() > 2)
		return (FAILURE);
	client->set_nickname((*(++(parsed_input.begin()))));
	send_nick(client, server, old_name);
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]