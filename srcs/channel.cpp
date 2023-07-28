#include <vector>
#include "channel.hpp"
#include "client.hpp"
#include "server.hpp"
#include "utils.hpp"

Channel::Channel(std::string name, Client* creator, Server* server) :
_server(server),
_owner(creator)
{
	_name = name;
	_topic = "";
	_password = "";
	_invite = false;
	_userlimit = 4096;
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
	_password = copy._password;
	_invite = copy._invite;
	_userlimit = copy._userlimit;
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

std::string	Channel::get_password() const
{
	return (_password);
}

bool		Channel::get_invite() const
{
	return (_invite);
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
	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
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

int		Channel::leave_channel(Client* client)
{
	remove_client(client);
	remove_operator(client, client);
	Client *owner = _owner;
	if (!(owner->get_fd() == client->get_fd()))
		return (SUCCESS); // Leave Channel
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
		add_operator(to_promote, to_promote);
		return (SUCCESS);
	}
	_server->remove_channel(get_name());
	return (SUCCESS);
}

int		Channel::set_topic(std::string topic, Client* client)
{
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_topic = topic;
	return (SUCCESS);
}

int		Channel::kick(Client* to_kick, Client* kicker)
{
	if (client_is_owner(to_kick->get_nickname()))
		return (CANT_KICK_OWNER);
	if (!client_is_operator(kicker->get_nickname()))
		return (REQUIRED_OPERATOR);
	if (!client_in_channel(to_kick->get_nickname()))
		return (NOT_IN_CHANNEL);
	to_kick->leave_channel(get_name());
	return (SUCCESS);
}

int		Channel::add_operator(Client* to_promote, Client* promoter)
{
	if (client_is_operator(to_promote->get_nickname()))
		return (ALREADY_OPERATOR);
	if (!(client_is_operator(promoter->get_nickname())))
		return (REQUIRED_OPERATOR);
	_operators.push_back(to_promote);
	return (SUCCESS);
}

int		Channel::remove_operator(Client* to_demote, Client* demoter)
{
	if(client_is_owner(to_demote->get_nickname()) && !client_is_owner(demoter->get_nickname()))
		return (UNABLE_TO_DEMOTE_OWNER);
	if (!(client_is_operator(demoter->get_nickname())))
		return (REQUIRED_OPERATOR);
	if (!(client_is_operator(to_demote->get_nickname())))
		return (NOT_AN_OPERATOR);
	for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->get_fd() == to_demote->get_fd())
		{
			_operators.erase(it);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

int		Channel::set_invite(Client* client, bool mode)
{
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_invite = mode;
	return (SUCCESS);	
}

int		Channel::set_password(std::string password, Client* client)
{
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	if (password != "" && name_syntax_check(password) != SUCCESS)
		return (NAME_SYNTAX_INVALID);
	_password = password;
	return (SUCCESS);
}

size_t		Channel::get_limit() const
{
	return (_userlimit);
}

int			Channel::set_limit(size_t limit, Client* client)
{
	if (!(client_is_operator(client->get_nickname())))
		return (REQUIRED_OPERATOR);
	//REPLACE WITH SERVERCONFIG
	if (limit > 4096)
		return (TOO_HIGH_OF_LIMIT);
	_userlimit = limit;
	return (SUCCESS);
}

//Setting Limit to 0 could fix it, but not sure, ask frans
// int		Channel::set_userlimit(size_t limit, Client* client)
// {
// 	if ()
// }
