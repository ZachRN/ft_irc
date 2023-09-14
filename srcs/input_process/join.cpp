#include <map>
#include <vector>
#include "server.hpp"

#include "utils.hpp"

static std::string clients_string(Channel* channel, std::vector<Client*> clientList)
{
	std::string all_clients;

	for (std::vector<Client*>::const_iterator client = clientList.begin(); client != clientList.end(); client++)
	{
		std::string client_nick = (*client)->get_nickname();
		if (channel->client_is_operator(client_nick) == true)
			all_clients += "@" + client_nick;
		else
			all_clients += client_nick;
		if (client + 1 != clientList.end())
			all_clients += " ";
	}
	return (all_clients);
}

static void send_channel_topic(Channel* channel, Client* to_send)
{
	if (channel->get_topic() == "")
		return ;
	channel->get_server()->send_msg(to_send->get_fd(), ":" + to_send->get_server()->get_config().get_host() + " 332 " + to_send->get_nickname() + " #" + channel->get_name() + " :" + channel->get_topic());
}

static void send_channel_clientlist(Channel* channel, Client* to_send)
{
	std::vector<Client*> clientList = channel->get_clients();
	std::string message = (":" + to_send->get_server()->get_config().get_host() +
						" 353 " +
						to_send->get_nickname() +
						" @ #" +
						channel->get_name() +
						" :" +
						clients_string(channel, clientList) +
						"\n");
	channel->get_server()->send_msg(to_send->get_fd(), message);
	message = (":" + to_send->get_server()->get_config().get_host() +
						" 366 " +
						to_send->get_nickname() +
						" #" +
						channel->get_name() +
						" :End of /NAMES list.\n");
	channel->get_server()->send_msg(to_send->get_fd(), message);
}

static void send_join(Client* client, Channel* channel, std::string channel_name)
{
	std::string message = (":" + client->get_fullref() + " JOIN #" + channel_name + "\n");
	channel->send_all_message(message);
}

// Join a single channel
int join_single_channel(Client* client, std::string channel_name, Server *server, std::string password)
{
	int retvalue = client->join_channel(channel_name, password);

	std::cout << "join_single_channel, channel name: " << channel_name << std::endl;
	switch (retvalue)
	{
		case PASSWORD_INVALID:
		{
			std::string message = (":" + server->get_config().get_host() + " 475 " + client->get_nickname() + " #" + channel_name + " :Cannot join channel (+k)\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case USER_LIMIT:
		{
			std::string message = (":" + server->get_config().get_host() + " 471 " + client->get_nickname() + " #" + channel_name + " :Cannot join channel (+l)\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case REQUIRED_INVITE:
		{
			std::string message = (":" + server->get_config().get_host() + " 473 " + client->get_nickname() + " #" + channel_name + " :Cannot join channel (+i)\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case ALREADY_IN_CHANNEL:
		{
			std::string message = (":" + server->get_config().get_host() + " 443 " + client->get_nickname() + " #" + channel_name + " :is already on channel\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case CHANNEL_NAME_IN_USE:
		{
			std::string message = (":" + server->get_config().get_host() + " 400 " + client->get_nickname() + " #" + channel_name + " :Channel name is already in use\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case NAME_SYNTAX_INVALID:
		{
			std::string message = (":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channel_name + " :No such channel\n");
			server->send_msg(client->get_fd(), message);
			return retvalue;
		}
		case SUCCESS:
		{
			send_join(client, server->get_channel(channel_name), channel_name);
			send_channel_topic(client->get_channel(channel_name), client);
			send_channel_clientlist(client->get_channel(channel_name), client);
			return (retvalue);
		}
		default:
			std::cerr << "Error: join_single_channel: " << retvalue << std::endl;
			return (FAILURE);
	}
}

// Join multiple channels, seperate by comma
int join(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() == 1)
		return (FAILURE);
	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	std::vector<std::string> channel_names = split((*(++(parsed_input.begin()))).substr(0), ',');
	if (parsed_input.size() > 3)
		return (FAILURE);

	// std::string password = (parsed_input.size() == 3) ? *(parsed_input.begin() + 2) : "";
	std::vector<std::string> passwords = (parsed_input.size() == 3) ? split(*(parsed_input.begin() + 2), ',') : std::vector<std::string>();
	size_t password_index = 0;
	for (std::vector<std::string>::const_iterator iter = channel_names.begin(); iter != channel_names.end(); ++iter)
	{
		std::string channel_name = *iter;
		channel_name = channel_name.substr(1);
		if (server->get_channel(channel_name) == nullptr && password_index < passwords.size())
		{
			join_single_channel(client, channel_name, server, passwords[password_index]);
			password_index++;
		}
		else if (server->get_channel(channel_name) == nullptr)
			join_single_channel(client, channel_name, server, "");
		else if (!server->get_channel(channel_name)->get_password().empty() && password_index < passwords.size())
		{
			join_single_channel(client, channel_name, server, passwords[password_index]);
			password_index++;
		}
		else if (password_index >= passwords.size())
			join_single_channel(client, channel_name, server, "");
		else
			join_single_channel(client, channel_name, server, "");
	}
	return (SUCCESS);
}
