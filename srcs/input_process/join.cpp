#include <map>
#include "server.hpp"
#include "run_server.hpp"

static std::string clients_string(Channel* channel, std::vector<Client*> clientList)
{
	std::string all_clients;

	for (std::vector<Client*>::const_iterator client = clientList.begin(); client != clientList.end(); client++)
	{
		std::string client_nick = (*client)->get_nickname();
		if (channel->client_is_operator(client_nick) == true)
			all_clients += "@" + client_nick;
		else
			all_clients += client_nick;
		if (client + 1 != clientList.end())
			all_clients += " ";
	}
	return (all_clients);
}

static void send_channel_clientlist(Channel* channel, Client* to_send)
{
	std::vector<Client*> clientList = channel->get_clients();
	std::string message = (":" + to_send->get_server()->get_config().get_host() +
						" 353 " +
						to_send->get_nickname() +
						" @ #" +
						channel->get_name() +
						" :" +
						clients_string(channel, clientList) +
						"\n");
	send_msg(to_send->get_fd(), message);
	message = (":" + to_send->get_server()->get_config().get_host() +
						" 366 " +
						to_send->get_nickname() +
						" #" +
						channel->get_name() +
						" :End of /NAMES list.\n");
	send_msg(to_send->get_fd(), message);
}

static void send_join(Client* client, Channel* channel, std::string channel_name)
{
	std::vector<Client*> clientList = channel->get_clients();
	std::string message = (":" + client->get_fullref() + " JOIN " + channel_name + "\n");
	for (std::vector<Client*>::const_iterator to_send = clientList.begin(); to_send != clientList.end(); to_send++)
	{
		send_msg((*to_send)->get_fd(), message);
	}
}

int join(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (parsed_input.size() > 3)
		return (FAILURE);
	std::string password = (parsed_input.size() == 3) ? *(parsed_input.begin() + 2) : "";
	int retvalue = client->join_channel(channel_name, password);
	if (retvalue != SUCCESS)
		return (retvalue);
	Channel*	channel = server->get_channel(channel_name);
	if (channel == nullptr)
		return (NO_CHANNEL_FOUND);
	send_join(client, channel, (*(++(parsed_input.begin()))));
	send_channel_clientlist(client->get_channel(channel_name), client);
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]