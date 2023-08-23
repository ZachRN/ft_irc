#include "server.hpp"
#include "run_server.hpp"

int	topic(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	std::string channelName = (parsed_input.size() > 1) ? (parsed_input.at(1).substr(1)) : "";
	if (channelName == "")
	{
		send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " TOPIC :Not enough parameters\n"));
		return (FAILURE);
	}
	Channel* channel = server->get_channel(channelName);
	if (channel == nullptr)
	{
		send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channelName + " :No such channel\n"));
		return (FAILURE);
	}
	if (!channel->client_is_operator(client->get_nickname()) && channel->get_topic_operator())
	{
		send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 482 " + client->get_nickname() + " #" + channelName + " :You're not channel operator\n"));
		return (FAILURE);
	}

	std::string topic = (parsed_input.size() > 2) ? (parsed_input.at(2)) : "";
	if (unparsed.find(":") != std::string::npos)
		topic = unparsed.substr(unparsed.find(":") + 1);
	channel->send_all_message(":" + client->get_fullref() + " TOPIC #" + channel->get_name() + " :" + topic);
	//:z77!~z88@185-61-55-68.185-61-55-ip.fmo-solutions.nl TOPIC #z99 :bwa ha ha
	return (channel->set_topic(topic, client));
}