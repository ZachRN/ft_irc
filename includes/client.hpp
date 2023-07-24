#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>

enum ClientErrors{
	FD_IN_USE = 1,
	NO_CLIENT_FOUND,
	NAME_ALREADY_INUSE,
	NAME_SYNTAX_INVALID
};

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
		int			_fd;
		std::string	_nickname;
		std::string	_username;
	public:
		Client(int fd);
		// Client(std::string nickname, std::string username);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &copy);
		
		int			get_fd() const;
		std::string	get_nickname() const;
		std::string	get_username() const;
		int			set_username(std::string username);
		int			set_nickname(std::string nickname);
};

#endif
