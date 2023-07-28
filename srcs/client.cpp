#include "client.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "utils.hpp"

//See Notes in Client.hpp for more information on commented out info.
Client::Client(int fd, Server* server) :
_server(server)
{
	_fd = fd;
	_nickname = "";
	_username = "";
	_verified = false;
	_channelList = std::vector<Channel*>();
	// std::cout << "bitch" << std::endl;
}

Client::Client(const Client &copy):
_server(copy._server)
{
	*this = copy;
}

Client::~Client()
{
	// std::cout << "ruhroh" << std::endl;
	return ;
}

Client	&Client::operator=(const Client &copy)
{
	_fd = copy._fd;
	_verified = copy._verified;
	_nickname = copy._nickname;
	_username = copy._username;
	_server = copy._server;
	_channelList = copy._channelList;
	return (*this);
}

int	Client::get_fd() const
{
	return (_fd);
}

std::string	Client::get_nickname() const
{
	return (_nickname);
}

int	Client::set_nickname(std::string nickname)
{
	if (name_syntax_check(nickname) == false)
		return (NAME_SYNTAX_INVALID);
	if (_server->nickname_in_use(nickname) == true)
		return (NAME_ALREADY_INUSE);
	_nickname = nickname;
	if (_username != "")
		_verified = true;
	return (SUCCESS);
}

std::string	Client::get_username() const
{
	return (_username);
}

int	Client::set_username(std::string username)
{
	if (name_syntax_check(username) == false)
		return (NAME_SYNTAX_INVALID);
	_username = username;
	if (_nickname != "")
		_verified = true;
	return (SUCCESS);
}

Server*		Client::get_server() const
{
	return (_server);
}

int			Client::join_channel(std::string channelName)
{
	std::map<std::string, Channel>* serverchannelList = _server->get_channelList();
	std::map<std::string, Channel>::iterator serverchannelIt = serverchannelList->find(channelName);
	if (is_in_channel(channelName) == true)
		return (FAILURE);
	if (serverchannelIt == serverchannelList->end())
	{
		if (_server->add_channel(channelName, *this) != SUCCESS)
			return (FAILURE);
		_channelList.push_back(&serverchannelList->find(channelName)->second);
		return (SUCCESS);
	}
	_channelList.push_back(&serverchannelIt->second);
	if (serverchannelIt->second.add_client(this) != SUCCESS)
	{
		_channelList.pop_back();
		return (FAILURE);
	}
	return (SUCCESS);
}

int			Client::leave_channel(std::string channelName)
{
	std::map<std::string, Channel>* channelList = _server->get_channelList();
	std::map<std::string, Channel>::iterator channelIt = channelList->find(channelName);
	if (channelIt == channelList->end())
		return (NO_CHANNEL_FOUND);
	if (channelIt->second.client_in_channel(get_nickname()) == true)
	{
		for (std::vector<Channel*>::iterator it = _channelList.begin(); it != _channelList.end(); it++)
		{
			if ((*it)->get_name() == channelName)
			{
				_channelList.erase(it);
				break;
			}
		}
		return (channelIt->second.leave_channel(this));
	}
	return (FAILURE);
}

std::vector<Channel*> Client::get_channelList(void)
{
	return (_channelList);
}

Channel*	Client::get_channel(std::string channelName)
{
	for(std::vector<Channel*>::iterator channelIt = _channelList.begin(); channelIt != _channelList.end(); channelIt++)
	{
		if ((*channelIt)->get_name() == channelName)
			return ((*channelIt));
	}
	return (nullptr);
}

bool	Client::is_in_channel(std::string channelName) const
{
	for(std::vector<Channel*>::const_iterator channelIt = _channelList.begin(); channelIt != _channelList.end(); channelIt++)
	{
		if ((*channelIt)->get_name() == channelName)
			return (true);
	}
	return (false);
}