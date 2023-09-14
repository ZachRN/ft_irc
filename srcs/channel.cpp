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
	_topic_operator = true;
	_password = "";
	_invite = false;
	_userlimit = 4096;
	_limitEnabled = false;
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
	_topic_operator = copy._topic_operator;
	_password = copy._password;
	_invite = copy._invite;
	_userlimit = copy._userlimit;
	_limitEnabled = copy._limitEnabled;
	_clients = copy._clients;
	_operators = copy._operators;
	_server = copy._server;
	_owner = copy._owner;
	_invitelist = copy._invitelist;
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

bool		Channel::get_topic_operator() const
{
	return (_topic_operator);
}

std::string	Channel::get_password() const
{
	return (_password);
}

bool		Channel::get_invite() const
{
	return (_invite);
}

Server*		Channel::get_server() const
{
	return (_server);
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
	if (client == nullptr)
		return (FAILURE);
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
	if (client == nullptr)
		return (FAILURE);
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
		send_all_message(":" +
						_server->get_config().get_host() +
						" MODE #" +
						_name +
						" +o " +
						to_promote->get_nickname() +
						"\n");
		return (SUCCESS);
	}
	_server->remove_channel(get_name());
	return (SUCCESS);
}

int		Channel::set_topic(std::string topic, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_topic = topic;
	return (SUCCESS);
}

int		Channel::kick(Client* to_kick, Client* kicker)
{
	if (to_kick == nullptr || kicker == nullptr)
		return (FAILURE);
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
	if (to_promote == nullptr || promoter == nullptr)
		return (FAILURE);
	if (client_is_operator(to_promote->get_nickname()))
		return (ALREADY_OPERATOR);
	if (!(client_is_operator(promoter->get_nickname()) || client_is_owner(promoter->get_nickname())))
		return (REQUIRED_OPERATOR);
	_operators.push_back(to_promote);
	return (SUCCESS);
}

int		Channel::remove_operator(Client* to_demote, Client* demoter)
{
	if (to_demote == nullptr || demoter == nullptr)
		return (FAILURE);
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

int		Channel::invite(Client* invitee, Client* inviter)
{
	if (invitee == nullptr || inviter == nullptr)
		return (FAILURE);
	if (!(client_is_operator(inviter->get_nickname())))
		return (REQUIRED_OPERATOR);
	_invitelist.push_back(invitee);
	return (SUCCESS);
}

int		Channel::remove_invite(Client* invitee, Client* inviter)
{
	if (invitee == nullptr || inviter == nullptr)
		return (FAILURE);
	if (!(client_is_operator(inviter->get_nickname())) && invitee != inviter)
		return (REQUIRED_OPERATOR);
	for (std::vector<Client *>::iterator it = _invitelist.begin(); it != _invitelist.end(); it++)
	{
		if ((*it)->get_fd() == invitee->get_fd())
		{
			_invitelist.erase(it);
			return (SUCCESS);
		}
	}
	return (FAILURE);
}

bool	Channel::is_invited(std::string nickname)
{
	for (std::vector<Client*>::const_iterator it = _invitelist.begin(); it != _invitelist.end(); it++)
	{
		if ((*it)->get_nickname() == nickname)
			return (true);
	}
	return (false);
}

int		Channel::set_invite(Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_invite = !_invite;
	return (SUCCESS);	
}

int		Channel::set_password(std::string password, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
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

bool		Channel::get_limit_enabled() const
{
	return (_limitEnabled);
}

int			Channel::set_limit(size_t limit, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (!(client_is_operator(client->get_nickname())))
		return (REQUIRED_OPERATOR);
	//REPLACE WITH SERVERCONFIG
	if (limit > client->get_server()->get_config().get_maxClients())
		return (TOO_HIGH_OF_LIMIT);
	_userlimit = limit;
	return (SUCCESS);
}

int		Channel::set_topic_operator(Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_topic_operator = !_topic_operator;
	return (SUCCESS);
}

int		Channel::set_flip_limit_enabled(Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (!client_is_operator(client->get_nickname()))
		return (REQUIRED_OPERATOR);
	_limitEnabled = !_limitEnabled;
	return (SUCCESS);
}

int			Channel::send_all_message(std::string message)
{
	for (std::vector<Client*>::const_iterator to_send = _clients.begin(); to_send != _clients.end(); to_send++)
	{
		_server->send_msg((*to_send)->get_fd(), message);
	}
	return (SUCCESS);
}

//Setting Limit to 0 could fix it, but not sure, ask frans
// int		Channel::set_userlimit(size_t limit, Client* client)
// {
// 	if ()
// }
