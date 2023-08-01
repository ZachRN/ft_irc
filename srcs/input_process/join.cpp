#include <map>
#include "server.hpp"
#include "run_server.hpp"

static void send_join(Client* client, Channel* channel, std::string channel_name)
{
	std::vector<Client*> clientList = channel->get_clients();
	std::string message = client->get_fullref();
	message.append(":zach!gmail JOIN ");
	message.append(channel_name);
	message.append("\n");
	for (std::vector<Client*>::const_iterator to_send = clientList.begin(); to_send != clientList.end(); to_send++)
	{
		send_msg((*to_send)->get_fd(), message);
	}
}

int join(Client* client, std::vector<std::string> parsed_input, Server *server)
{
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
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]