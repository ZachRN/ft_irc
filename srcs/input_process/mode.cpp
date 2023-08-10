#include "server.hpp"
#include "run_server.hpp"

static int invite_mode(Client* client, Channel* channel, std::string mode)
{

}

static int topic_mode(Client* client, Channel* channel, std::string mode)
{

}

static int password_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode)
{
	std::string password = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "";
	if (mode == "-k")
	{
		channel->set_password("", client);
		channel->send_all_message("MESSAGE GOES HERE");
	}
	else
	{
		channel->set_password(password, client);
		channel->send_all_message("MESSAGE GOES HERE");
	}
	return (SUCCESS);
}

static int operator_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode, Server* server)
{
	std::string user = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "";
	if (user == "")
	{
		//:zirconium.libera.chat 461 zachary123 MODE :Not enough parameters
		send_msg(client->get_fd(), "not enough params error here");
		return (FAILURE);
	}
	int retvalue = 0;
	if (mode == "o")
	{
		retvalue = channel->add_operator(server->get_client(user), client);
		if (retvalue == SUCCESS)
			channel->send_all_message("OPERATOR PROMOTED MESSAGE HERE");
	}
	if (mode == "-o")
	{
		retvalue = channel->add_operator(server->get_client(user), client);
		if (retvalue == SUCCESS)
			channel->send_all_message("OPERATOR PROMOTED MESSAGE HERE");
	}
	if (retvalue != SUCCESS)
	{
		//SWITCH CASE HERE TO SEND ERROR MSG
	}
}

static int limit_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode)
{
	std::string limit = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "";
	if (limit.find_first_not_of("0123456789") != std::string::npos)
	{
		send_msg(client->get_fd(), "ERROR GOES HERE");
		return (FAILURE);
	}
	channel->send_all_message("SET LIMIT MESSAGE GOES HERE");
	return (SUCCESS);
}

int mode(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	Channel* channel = server->get_channel(channel_name);
	if (channel == nullptr)
		return (NO_CHANNEL_FOUND);
	if (parsed_input.size() < 2)
		return (FAILURE);
	std::string mode = parsed_input.at(2);
	if (mode == "i" || mode == "-i")
		return (invite_mode(client, channel, mode));
	if (mode == "t" || mode == "-t")
		return (topic_mode(client, channel, mode));
	if (mode == "k" || mode == "-k")
		return (password_mode(client, parsed_input, channel, mode));
	if (mode == "o" || mode == "-o")
		return (operator_mode(client, parsed_input, channel, mode, server));
	if (mode == "l" || mode == "-l")
		return (limit_mode(client, parsed_input, channel, mode));
}