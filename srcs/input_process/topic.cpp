#include "server.hpp"
#include "run_server.hpp"

static void send_channel_topic(Channel* channel, Client* to_send)
{
	if (channel->get_topic() == "")
		return ;
	channel->get_server()->send_msg(to_send->get_fd(), ":" + to_send->get_server()->get_config().get_host() + " 332 " + to_send->get_nickname() + " #" + channel->get_name() + " :" + channel->get_topic());
}

int	topic(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed)
{
	std::string channelName = (parsed_input.size() > 1) ? (parsed_input.at(1).substr(1)) : "";
	if (channelName == "")
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " TOPIC :Not enough parameters\n"));
		return (FAILURE);
	}
	Channel* channel = server->get_channel(channelName);
	if (channel == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channelName + " :No such channel\n"));
		return (FAILURE);
	}
	if (!channel->client_is_operator(client->get_nickname()) && channel->get_topic_operator())
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 482 " + client->get_nickname() + " #" + channelName + " :You're not channel operator\n"));
		return (FAILURE);
	}

	std::string topic = (parsed_input.size() > 2) ? (parsed_input.at(2)) : "";
	if (unparsed.find(":") != std::string::npos)
		topic = unparsed.substr(unparsed.find(":") + 1);
	else
	{
		send_channel_topic(channel, client);
		return (SUCCESS);
	}
	if (topic[topic.size() - 1] != '\n')
		topic += "\n";
	channel->send_all_message(":" + client->get_fullref() + " TOPIC #" + channel->get_name() + " :" + topic);
	return (channel->set_topic(topic, client));
}