#include "server.hpp"
#include "run_server.hpp"

int	kick(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	std::string channelName = (parsed_input.size() > 2) ? (parsed_input.at(1).substr(1)) : "";
	if (channelName == "")
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " KICK :Not enough parameters\n"));
		return (FAILURE);
	}
	Channel* channel = server->get_channel(channelName);
	if (channel == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channelName + " :No such channel\n"));
		return (FAILURE);
	}
	if (!channel->client_is_operator(client->get_nickname()))
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 482 " + client->get_nickname() + " #" + channelName + " :You're not channel operator\n"));
		return (FAILURE);
	}
	Client* kickee = server->get_client(parsed_input.at(2));
	if (kickee == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 401 " + client->get_nickname() + " " + parsed_input.at(2) + " :No such nick/channel\n"));
		return (FAILURE);
	}
	switch (channel->kick(kickee, client))
	{
		case CANT_KICK_OWNER:
			server->send_msg(client->get_fd(), ":" +server->get_config().get_serverName()+ " 400 " + client->get_nickname() + " :Unknown Can't Kick Owner.\n");
			break;
		case NOT_IN_CHANNEL:
			server->send_msg(client->get_fd(), ":" +server->get_config().get_serverName()+ " 400 " + client->get_nickname() + " :Unknown Can't Kick Someone Not in Channel.\n");
			break;
		default:
			break;
	}
	std::string reason = "";
	if (unparsed.find(":") != std::string::npos)
		reason = unparsed.substr(unparsed.find(":") + 1);
	std::string kickMsg = ":" + client->get_nickname() + " KICK #" + channel->get_name() + " " + kickee->get_nickname() + " :" + reason;
	channel->send_all_message(kickMsg);
	server->send_msg(kickee->get_fd(), kickMsg);
	return (SUCCESS);
}