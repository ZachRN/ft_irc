#include <map>
#include "server.hpp"
#include "run_server.hpp"

static void send_join(Client* client, Channel* channel, std::string channel_name)
{
	std::string message = (":" + client->get_fullref() + " JOIN " + channel_name + "\n");
	channel->send_all_message(message);
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
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]