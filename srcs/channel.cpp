#include "channel.hpp"
#include "client.hpp"
#include "server.hpp"
#include <vector>

Channel::Channel(std::string name, Client &creator)
{
	_name = name;
	_clients.push_back(creator);
	_operators.push_back(creator);
}

Channel::Channel(const Channel &copy)
{
	*this = copy;
}

Channel::~Channel()
{
	return ;
}

Channel	&Channel::operator=(const Channel &copy)
{
	_name = copy._name;
	_topic = copy._topic;
	_clients = copy._clients;
	_operators = copy._operators;
	return (*this);
}

std::string	Channel::get_name() const
{
	return (_name);
}

std::string	Channel::get_topic() const
{
	return (_topic);
}

std::vector<Client>	Channel::get_clients() const
{
	return (_clients);
}

bool Channel::client_in_channel(std::string nickname) const
{
	for (std::vector<Client>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it.base()->get_nickname() == nickname)
			return (true);
	}
	return (false);
}

std::vector<Client>	Channel::get_operators() const
{
	return (_operators);
}

bool		Channel::add_client(Client &client)
{
	_clients.push_back(client);
	return (SUCCESS);
}

bool		Channel::remove_client(Client &client)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it.base()->get_fd() == client.get_fd())
		{
			_clients.erase(it);
			return (true);
		}
	}
	return (false);
}

bool		Channel::add_operator(Client &client)
{
	_operators.push_back(client);
	return (true);
}

bool		Channel::remove_operator(Client &client)
{
	for (std::vector<Client>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (it.base()->get_fd() == client.get_fd())
		{
			_operators.erase(it);
			return (true);
		}
	}
	return (false);
}
