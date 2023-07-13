#include "channel.hpp"
#include "client.hpp"
#include <vector>

Channel::Channel(std::string name, Client *creator)
{
	_name = name;
	_creator = creator;
	_clients.push_back(creator);
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
	_creator = copy._creator;
	_clients = copy._clients;
	return (*this);
}

std::string	Channel::get_name() const
{
	return (_name);
}

Client		*Channel::get_creator() const
{
	return (_creator);
}

std::vector<Client *>	Channel::get_clients() const
{
	return (_clients);
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
