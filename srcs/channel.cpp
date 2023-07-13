#include "channel.hpp"
#include "client.hpp"
#include <vector>

Channel::Channel(std::string name, Client *creator)
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

std::vector<Client *>	Channel::get_clients() const
{
	return (_clients);
}

std::vector<Client *>	Channel::get_operators() const
{
	return (_operators);
}

bool		Channel::add_client(Client *client)
{
	_clients.push_back(client);
	return (true);
}

bool		Channel::remove_client(Client *client)
{
	std::vector<Client *>::iterator it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		if (*it == client)
		{
			_clients.erase(it);
			return (true);
		}
		it++;
	}
	return (false);
}

bool		Channel::add_operator(Client *client)
{
	_operators.push_back(client);
	return (true);
}

bool		Channel::remove_operator(Client *client)
{
	std::vector<Client *>::iterator it;

	it = _operators.begin();
	while (it != _operators.end())
	{
		if (*it == client)
		{
			_operators.erase(it);
			return (true);
		}
		it++;
	}
	return (false);
}
