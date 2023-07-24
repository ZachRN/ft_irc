#include "client.hpp"
#include "server.hpp"
#include "utils.hpp"

//See Notes in Client.hpp for more information on commented out info.
Client::Client(int fd)
{
	_fd = fd;
}
// Client::Client(std::string nickname, std::string username)
// {
// 	//!!NEED TO CHECK BOTH USERNAME AND NICKNAME, IF EITHER ARE INVALID
// 	//WE MIGHT NEED TO THROW DURING CONSTRUCTION; WHICH IS AN ISSUE, OR WE HAVE A
// 	//"ZOMBIE" STYLE STATE AS A WORKAROUND, SINCE CONSTRUCTORS CANT RETURN.
// 	_nickname = nickname;
// 	_username = username;
// }

// Client::Client(std::string nickname, std::string realname, std::string username, Server *server)
// {
// 	_nickname = nickname;
// 	_realname = realname;
// 	_username = username;
// 	_server = server;
// }

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
	_username = copy._username;
	return (*this);
}

// Client	&Client::operator=(const Client &copy)
// {
// 	_nickname = copy._nickname;
// 	_realname = copy._realname;
// 	_username = copy._username;
// 	_server = copy._server;
// 	return (*this);
// }

std::string	Client::get_nickname() const
{
	return (_nickname);
}

// std::string	Client::get_realname() const
// {
// 	return (_realname);
// }

std::string	Client::get_username() const
{
	return (_username);
}

int	Client::set_username(std::string username)
{
	if (name_syntax_check(username) == false)
		return (NAME_SYNTAX_INVALID);
	_username = username;
	return (SUCCESS);
}

// Server		*Client::get_server() const
// {
// 	return (_server);
// }

// bool		Client::set_nickname(std::string nickname)
// {
// 	/* 
// 		Change this to proper nickname validation
// 	*/
// 	if (nickname.length() > 9)
// 		return (false);
// 	_nickname = nickname;
// 	return (true);
// }
