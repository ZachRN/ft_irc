#include "server.hpp"
#include "run_server.hpp"

int invite(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	std::string user = (parsed_input.size() > 2) ? (parsed_input.at(1)) : "";
	if (user == "")
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " INVITE :Not enough parameters\n"));
		return (FAILURE);
	}
	std::string channel_name = parsed_input.at(2).substr(1);
	Channel* channel = server->get_channel(channel_name);
	if (channel == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channel_name + " :No such channel\n"));
		return (NO_CHANNEL_FOUND);
	}
	if (channel->client_is_operator(client->get_nickname()) == false)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 482 " + client->get_nickname() + " #" + channel_name + " :You're not channel operator\n"));
		return (REQUIRED_OPERATOR);
	}
	Client* invitee = server->get_client(user);
	if (invitee == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 401 " + client->get_nickname() + " " + user + " :No such nick/channel\n"));
		return (NO_CLIENT_FOUND);
	}
	channel->invite(invitee, client);
	server->send_msg(client->get_fd(), (":" + server->get_config().get_serverName() + " 341 " + client->get_nickname() + " " + invitee->get_nickname() + " #" + channel->get_name() + "\n"));
	server->send_msg(invitee->get_fd(), (":" + client->get_fullref() + " INVITE " + invitee->get_nickname() + " :#" + channel->get_name() + "\n"));
	return (SUCCESS);
}
