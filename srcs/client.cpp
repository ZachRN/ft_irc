#include "client.hpp"
#include "server.hpp"

Client::Client(std::string nickname, std::string realname, std::string username, Server *server)
{
	_nickname = nickname;
	_realname = realname;
	_username = username;
	_server = server;
}

Client::Client(const Client &copy)
{
	*this = copy;
}

Client::~Client()
{
	return ;
}

Client	&Client::operator=(const Client &copy)
{
	_nickname = copy._nickname;
	_realname = copy._realname;
	_username = copy._username;
	_server = copy._server;
	return (*this);
}

std::string	Client::get_nickname() const
{
	return (_nickname);
}

std::string	Client::get_realname() const
{
	return (_realname);
}

std::string	Client::get_username() const
{
	return (_username);
}

Server		*Client::get_server() const
{
	return (_server);
}

bool		Client::set_nickname(std::string nickname)
{
	/* 
		Change this to proper nickname validation
	*/
	if (nickname.length() > 9)
		return (false);
	_nickname = nickname;
	return (true);
}
