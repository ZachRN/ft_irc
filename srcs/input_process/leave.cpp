#include <map>
#include "server.hpp"
#include "run_server.hpp"

void send_leave(Client* client, Channel* channel, std::string channel_name)
{
	std::string message = (":" + client->get_fullref() + " PART " + channel_name + "\n");
	channel->get_server()->send_msg(client->get_fd(), message);
	
	if (channel == nullptr)
		return ;
	channel->send_all_message(message);
}

int leave(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (parsed_input.size() > 4)
		return (FAILURE);
	Channel*	channel = server->get_channel(channel_name);
	if (channel == nullptr)
		return (NO_CHANNEL_FOUND);
	if (client->leave_channel(channel_name) != SUCCESS)
		return (FAILURE);
	channel = server->get_channel(channel_name);
	if (channel == nullptr)
		return (SUCCESS);
	channel = server->get_channel(channel_name);
	send_leave(client, channel, (*(++(parsed_input.begin()))));
	return (SUCCESS);
}
