#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "server.hpp"
/*
	1.2 Clients

	A client is anything connecting to a server that is not another
	server.  Each client is distinguished from other clients by a unique
	nickname having a maximum length of nine (9) characters.  See the
	protocol grammar rules for what may and may not be used in a
	nickname.  In addition to the nickname, all servers must have the
	following information about all clients: the real name of the host
	that the client is running on, the username of the client on that
	host, and the server to which the client is connected.
*/

class Client
{
	private:
		std::string	_nickname;
		std::string	_realname;
		std::string	_username;
		Server		*_server;
	public:
		Client(std::string nickname, std::string realname, std::string username, Server *server);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &copy);
		
		std::string	get_nickname() const;
		std::string	get_realname() const;
		std::string	get_username() const;
		Server		*get_server() const;

		bool		set_nickname(std::string nickname);
};

#endif
