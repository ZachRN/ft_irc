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
	if (parsed_input.size() != 2)
		return (FAILURE);
	std::string old_name = client->get_nickname();
	bool first_name = old_name == "" ? true : false;
	int retvalue = 0;
	retvalue = client->set_nickname((*(++(parsed_input.begin()))));
	switch (retvalue)
	{
		case NAME_ALREADY_INUSE:
		{
			server->send_msg(client->get_fd(),((":" + server->get_config().get_host() + " 433 * " + (*(++(parsed_input.begin()))) + " :Nickname is already in use.\n")));
			return (retvalue);
		}
		case SUCCESS:
		{}
	};
	client->set_nickname((*(++(parsed_input.begin()))));
	if (!first_name)
		send_nick(client, server, old_name);
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]