#include "server.hpp"
#include "input_process.hpp"

static void send_msg(Client* client, std::vector<std::string> parsed_input, Channel *channel)
{

}

static void send_single_msg(Client* client, std::string to_send, Channel *channel)
{
	
}

int privmsg(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	Channel*	channel = server->get_channel(channel_name);
	if (channel == nullptr)
		return (NO_CHANNEL_FOUND);
	if (pasrsed_input.size() == 3)
	{
		
	}
	return (SUCCESS);
}