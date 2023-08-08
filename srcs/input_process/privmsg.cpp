#include "run_server.hpp"
#include "server.hpp"
#include "input_process.hpp"
#include "utils.hpp"

static void send_channel_msg(Client* client, std::vector<std::string> parsed_input, Channel *channel, std::string unparsed)
{
	size_t	colon_pos = unparsed.find_first_of(":");
	std::string msg = "";
	if (colon_pos != std::string::npos)
		msg = unparsed.substr(colon_pos + 1) + "\n";
	else
		msg = parsed_input.at(2);
	std::string the_message = ":" + client->get_fullref() + " PRIVMSG " + "#" + channel->get_name() + " :" + msg + "\n";
	std::cout << the_message << std::endl;
	std::vector<Client*> clientList = channel->get_clients();
	for (std::vector<Client*>::const_iterator to_send = clientList.begin(); to_send != clientList.end(); to_send++)
	{
		send_msg((*to_send)->get_fd(), the_message);
	}
}

int privmsg(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	Channel*	channel = server->get_channel(channel_name);
	unparsed = trim_white_space(unparsed);
	if (channel == nullptr)
		return (NO_CHANNEL_FOUND);
	if (parsed_input.size() < 3)
		return (FAILURE);
	send_channel_msg(client, parsed_input, channel, unparsed);
	return (SUCCESS);
}