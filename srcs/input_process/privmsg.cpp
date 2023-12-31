
#include "server.hpp"
#include "input_process.hpp"
#include "utils.hpp"

static void send_channel_msg(Client* client, std::vector<std::string> parsed_input, Channel *channel, std::string unparsed)
{
	size_t	colon_pos = unparsed.find_first_of(":");
	std::string msg = "";
	if (colon_pos != std::string::npos)
		msg = unparsed.substr(colon_pos + 1);
	else
		msg = parsed_input.at(2);
	std::string the_message = ":" + client->get_fullref() + " PRIVMSG " + "#" + channel->get_name() + " :" + msg + "\n";
	std::cout << the_message << std::endl;
	std::vector<Client*> clientList = channel->get_clients();
	for (std::vector<Client*>::const_iterator to_send = clientList.begin(); to_send != clientList.end(); to_send++)
	{
		if ((*to_send)->get_fd() != client->get_fd())
			channel->get_server()->send_msg((*to_send)->get_fd(), the_message);
	}
}

static int	send_private_message(Client* client, std::string name, Server *server, std::vector<std::string> parsed_input, std::string unparsed)
{
	Client* to_send = server->get_client(name);
	if (to_send == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 401 " + client->get_nickname() + " " + parsed_input.at(2) + " :No such nick/channel\n"));
		return (FAILURE);
	}
	size_t	colon_pos = unparsed.find_first_of(":");
	std::string msg = "";
	if (colon_pos != std::string::npos)
		msg = unparsed.substr(colon_pos + 1);
	else
		msg = parsed_input.at(2);
	std::string the_message = ":" + client->get_fullref() + " PRIVMSG " + to_send->get_nickname() + " :" + msg + "\n";
	server->send_msg(to_send->get_fd(), the_message);
	return (SUCCESS);
}

int privmsg(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	if (parsed_input.size() < 3)
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " INVITE :Not enough parameters\n"));
		return (FAILURE);
	}
	unparsed = trim_whitespace(unparsed);
	if ((*(++(parsed_input.begin()))).at(0) != '#')
		return (send_private_message(client, (*(++(parsed_input.begin()))), server, parsed_input, unparsed));
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	Channel*	channel = server->get_channel(channel_name);
	if (channel == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channel_name + " :No such channel\n"));
		return (NO_CHANNEL_FOUND);
	}
	if (channel->client_in_channel(client->get_nickname()) == false)
	{
		server->send_msg(client->get_fd(), ":" + server->get_config().get_host() + " 400 " + client->get_nickname() + " #" + channel_name + " :Unknown You aren't in channel.\n");
		return (FAILURE);
	}
	send_channel_msg(client, parsed_input, channel, unparsed);
	return (SUCCESS);
}
