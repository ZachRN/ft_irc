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
	_passCorrect = false;
	_channelList = std::vector<Channel*>();
	_lastPong = std::time(nullptr);
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
	_lastPong = copy._lastPong;
	_channelList = copy._channelList;
	return (*this);
}

void	Client::set_verified()
{
	if (get_verified() == true)
		return ;
	_verified = true;
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 001 " + get_nickname() + " :Welcome to the Internet Relay Network " + _fullRef + "\n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 002 " + get_nickname() + " :Your host is " + _server->get_config().get_host() + ", running version " + _server->get_config().get_version() + "\n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 003 " + get_nickname() + " :This server was created sometime\n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 004 " + get_nickname() + " " + _server->get_config().get_host() + " 1.0 o o\n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 375 " + get_nickname() + " :- localhost Message of the Day - \n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 372 " + get_nickname() + " :- Welcome to the Internet Relay Network.\n"));
	_server->send_msg(get_fd(), (":" + _server->get_config().get_serverName() + " 376 " + get_nickname() + " :End of MOTD command\n"));
}

int	Client::get_fd() const
{
	return (_fd);
}

std::string	Client::get_fullref() const
{
	return (_fullRef);
}

std::string	Client::get_nickname() const
{
	return (_nickname);
}

int	Client::set_nickname(std::string nickname)
{
	if (name_syntax_check(nickname) != SUCCESS)
		return (NAME_SYNTAX_INVALID);
	if (_server->nickname_in_use(nickname) == true)
		return (NAME_ALREADY_INUSE);
	_nickname = nickname;
	if (_username == "")
		return (SUCCESS);
	_fullRef = _nickname + "!" + _username + "@" + _server->get_config().get_serverName();
	set_verified();
	return (SUCCESS);
}

std::string	Client::get_username() const
{
	return (_username);
}

int	Client::set_username(std::string username)
{
	if (name_syntax_check(username) != SUCCESS)
		return (NAME_SYNTAX_INVALID);
	_username = username;
	if (_nickname== "")
		return (SUCCESS);
	_fullRef = _nickname + "!" + _username + "@" + _server->get_config().get_serverName();
	set_verified();
	return (SUCCESS);
}

bool		Client::get_verified() const
{
	return (_verified);
}

bool		Client::get_passCorrect() const
{
	return (_passCorrect);
}

int			Client::set_passCorrect(std::string password)
{
	if (get_verified() == true)
		return (SUCCESS);
	if (!(password == get_server()->get_pass()))
	{
		_passCorrect = false;
		return (FAILURE);
	}
	_passCorrect = true;
	return (SUCCESS);
}


Server*		Client::get_server() const
{
	return (_server);
}

std::time_t	Client::get_lastPong() const
{
	return (_lastPong);
}

void		Client::set_lastPong(std::time_t lastPong)
{
	_lastPong = lastPong;
}

void		Client::push_message(std::string message)
{
	_messageQueue.push_back(message);
}

std::string	Client::pop_message()
{
	std::string message = _messageQueue.front();
	_messageQueue.pop_front();
	return (message);
}

bool		Client::has_messages() const
{
	return (_messageQueue.size() > 0);
}

void		Client::add_bufferedInput(std::string input)
{
	_bufferedInput += input;
}

std::string	Client::get_bufferedInput()
{
	std::string input = _bufferedInput;
	_bufferedInput.clear();
	return (input);
}

static int	can_join_channel(Channel* channel, std::string password, std::string nickname)
{
	if (channel == nullptr)
		return (FAILURE);
	if (channel->get_password() != password)
		return (PASSWORD_INVALID);
	if (channel->get_clients().size() >= channel->get_limit() && channel->get_limit() != 0)
		return (USER_LIMIT);
	if (channel->get_invite() == true && channel->is_invited(nickname) == false)
		return (REQUIRED_INVITE);
	return (SUCCESS);
}

int			Client::join_channel(std::string channelName, std::string password)
{
	std::map<std::string, Channel>* serverchannelList = _server->get_channelList();
	std::map<std::string, Channel>::iterator serverchannelIt = serverchannelList->find(channelName);
	if (is_in_channel(channelName) == true)
		return (ALREADY_IN_CHANNEL);
	if (serverchannelIt == serverchannelList->end())
	{
		int error_code = _server->add_channel(channelName, *this);
		if (error_code != SUCCESS)
			return (error_code);
		_channelList.push_back(&serverchannelList->find(channelName)->second);
		return (SUCCESS);
	}
	int retvalue = can_join_channel(&serverchannelIt->second, password, get_nickname());
	if (retvalue != SUCCESS)
		return (retvalue);
	_channelList.push_back(&serverchannelIt->second);
	if (serverchannelIt->second.add_client(this) != SUCCESS)
	{
		_channelList.pop_back();
		return (FAILURE);
	}
	serverchannelIt->second.remove_invite(this, this);
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