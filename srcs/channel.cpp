#include "channel.hpp"
#include "client.hpp"
#include "server.hpp"
#include <vector>

Channel::Channel(std::string name, Client* creator, Server* server) :
_server(server),
_owner(creator)
{
	_name = name;
	_clients.push_back(creator);
	_operators.push_back(creator);
}

Channel::Channel(const Channel &copy) :
_server(copy._server),
_owner(copy._owner)
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
	_server = copy._server;
	_owner = copy._owner;
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

std::vector<Client*>	Channel::get_clients() const
{
	return (_clients);
}

bool Channel::client_in_channel(std::string nickname) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->get_nickname() == nickname)
			return (true);
	}
	return (false);
}

std::vector<Client*>	Channel::get_operators() const
{
	return (_operators);
}

bool	Channel::client_is_operator(std::string nickname) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->get_nickname() == nickname)
			return (true);
	}
	return (false);
}

Client*		Channel::get_owner() const
{
	return (_owner);
}

bool	Channel::client_is_owner(std::string nickname) const
{
	if (_owner->get_nickname() == nickname)
		return (true);
	return (false);
}

bool		Channel::add_client(Client* client)
{
	_clients.push_back(client);
	return (SUCCESS);
}

bool		Channel::remove_client(Client* client)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)->get_fd() == client->get_fd())
		{
			_clients.erase(it);
			return (true);
		}
	}
	return (false);
}

bool		Channel::add_operator(Client* client)
{
	_operators.push_back(client);
	return (true);
}

bool		Channel::remove_operator(Client* client)
{
	for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->get_fd() == client->get_fd())
		{
			_operators.erase(it);
			return (true);
		}
	}
	return (false);
}

int		Channel::leave_channel(Client* client)
{
	remove_client(client);
	remove_operator(client);
	Client *owner = _owner;
	if (!(owner->get_fd() == client->get_fd()))
	{
		return (SUCCESS); // Leave Channel
	}
	std::vector<Client*> channeloperatorList = get_operators();
	if (!channeloperatorList.empty())
	{
		Client *to_promote = channeloperatorList.front();
		_owner = to_promote;
		return (SUCCESS);
	}
	std::vector<Client*> channelclientList = get_clients();
	if (!channelclientList.empty())
	{
		Client *to_promote = channelclientList.front();
		_owner = to_promote;
		add_operator(to_promote);
		return (SUCCESS);
	}
	_server->remove_channel(get_name());
	return (SUCCESS);
}
