#include "server.hpp"
#include "run_server.hpp"
#include "utils.hpp"

static int invite_mode(Client* client, Channel* channel, std::string mode)
{
	int retvalue = 0;
	std::string message = ":" + client->get_fullref() + " MODE #" + channel->get_name();
	if (mode == "i" || mode == "+i")
	{
		if (channel->get_invite() == false)
		{
			retvalue = channel->set_invite(client);
			if (retvalue == SUCCESS)
				channel->send_all_message(message + " +i\n");
		}
	}
	if (mode == "-i")
	{
		if (channel->get_invite() == true)
		{
			retvalue = channel->set_invite(client);
			if (retvalue == SUCCESS)
				channel->send_all_message(message + " -i\n");
		}
	}
	return (SUCCESS);
}

static int topic_mode(Client* client, Channel* channel, std::string mode)
{
	int retvalue = 0;
	std::string message = ":" + client->get_fullref() + " MODE #" + channel->get_name();
	if (mode == "t" || mode == "+t")
	{
		if (channel->get_topic_operator() == false)
		{
			retvalue = channel->set_topic_operator(client);
			if (retvalue == SUCCESS)
				channel->send_all_message(message + " +t\n");
		}
	}
	if (mode == "-t")
	{
		if (channel->get_topic_operator() == true)
		{
			retvalue = channel->set_topic_operator(client);
			if (retvalue == SUCCESS)
				channel->send_all_message(message + " -t\n");
		}
	}
	return (SUCCESS);
}

static int password_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode)
{
	std::string password = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "";
	std::string message = ":" + client->get_fullref() + " MODE #" + channel->get_name();
	int retvalue = 0;
	if (mode == "k" || mode == "+k")
	{
		retvalue = channel->set_password(password, client);
		if (retvalue == SUCCESS)
			channel->send_all_message(message + " +k " + password + "\n");
	}
	else
	{
		retvalue = channel->set_password("", client);
		if (retvalue == SUCCESS)
			channel->send_all_message(message + " -k\n");
	}
	if (retvalue == NAME_SYNTAX_INVALID)
		channel->get_server()->send_msg(client->get_fd(), ":" + client->get_fullref() + " 464 " + password + " :Password incorrect\n");
	return (SUCCESS);
}

static int operator_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode, Server* server)
{
	std::string user = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "";
	std::string message = ":" + client->get_fullref() + " MODE #" + channel->get_name();
	if (user == "")
	{
		server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 461 " + client->get_nickname() + " MODE :Not enough parameters\n"));
		return (FAILURE);
	}
	if (server->get_client(user) == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 401 " + client->get_nickname() + " " + user + " :No such nick/channel\n"));
		return (FAILURE);
	}
	int retvalue = 0;
	if (mode == "o" || mode == "+o")
	{
		retvalue = channel->add_operator(server->get_client(user), client);
		if (retvalue == SUCCESS)
			channel->send_all_message((message + " +o " + server->get_client(user)->get_nickname() + "\n"));
	}
	if (mode == "-o")
	{
		retvalue = channel->remove_operator(server->get_client(user), client);
		if (retvalue == SUCCESS)
			channel->send_all_message((message + " -o " + server->get_client(user)->get_nickname() + "\n"));
	}
	return (SUCCESS);
}

static int limit_mode(Client* client, std::vector<std::string> parsed_input, Channel* channel, std::string mode, Server* server)
{
	std::string limit = (parsed_input.size() > 3) ? (parsed_input.at(3)) : "1";
	if (limit.size() > 4)
	{
		server->send_msg(client->get_fd(), ":" +server->get_config().get_serverName()+ " 400 " + client->get_nickname() + " :Unknown Set limit over the config limit.\n");
		return (FAILURE);
	}
	if (limit.find_first_not_of("0123456789") != std::string::npos)
	{
		server->send_msg(client->get_fd(), ":" +server->get_config().get_serverName()+ " 400 " + client->get_nickname() + " :Unknown Limit didn't contain only numeric characters.\n");
		return (FAILURE);
	}
	int numLimit = -1;
	try{numLimit = std::stoi(limit);}
	catch (...){numLimit = -1;}
	//MAYBE > INSTEAD OF >=
	if (numLimit >= static_cast<int>(server->get_config().get_maxClients()) || numLimit < 1)
	{
		server->send_msg(client->get_fd(), ":" + server->get_config().get_serverName()+ " 400 " + client->get_nickname() + " :Unknown Set limit over the config limit or under 1.\n");
		return (FAILURE);
	}
	std::string message = ":" + client->get_fullref() + " MODE #" + channel->get_name();
	if (mode == "l" || mode == "+l")
	{
		if (channel->set_limit(numLimit, client) == SUCCESS)
		{
			if (channel->get_limit() == false)
				channel->set_flip_limit_enabled(client);
			channel->send_all_message((message + " +l " + limit + "\n"));
		}
	}
	if (mode == "-l")
	{
		if (channel->set_limit(server->get_config().get_maxClients(), client) == SUCCESS)
		{
			if (channel->get_limit() == true)
				channel->set_flip_limit_enabled(client);
			channel->send_all_message((message + " -l\n"));
		}
	}
	return (SUCCESS);
}

int mode(Client* client, std::vector<std::string> parsed_input, Server *server)
{
	if (parsed_input.size() < 3)
		return (FAILURE);
	std::string channel_name = parsed_input.at(1).substr(1);
	Channel* channel = server->get_channel(channel_name);
	if (channel == nullptr)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 403 " + client->get_nickname() + " #" + channel_name + " :No such channel\n"));
		return (NO_CHANNEL_FOUND);
	}
	if (channel->client_is_operator(client->get_nickname()) == false)
	{
		server->send_msg(client->get_fd(),(":" + server->get_config().get_host() + " 482 " + client->get_nickname() + " #" + channel_name + " :You're not channel operator\n"));
		return (NO_CHANNEL_FOUND);
	}
	std::string mode = trim_whitespace(parsed_input.at(2));
	if (mode == "i" || mode == "-i" || mode == "+i")
		return (invite_mode(client, channel, mode));
	if (mode == "t" || mode == "-t" || mode == "+t")
		return (topic_mode(client, channel, mode));
	if (mode == "k" || mode == "-k" || mode == "+k")
		return (password_mode(client, parsed_input, channel, mode));
	if (mode == "o" || mode == "-o" || mode == "+o")
		return (operator_mode(client, parsed_input, channel, mode, server));
	if (mode == "l" || mode == "-l" || mode == "+l")
		return (limit_mode(client, parsed_input, channel, mode, server));
	if (mode == "+sn")
		return (FAILURE);
	server->send_msg(client->get_fd(), (":" + server->get_config().get_host() + " 421 " + mode + " :Unknown Command\n"));
	return (FAILURE);
}