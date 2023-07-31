#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <ctime>
# include <iostream>
# include <string>
# include <vector>
// # include "server.hpp"

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

class Server;
class Channel;

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		//This says if the setup process of their account is complete.
		bool		_verified;
		//std::time_t	_lastPong; Reinstate this later
		Server		*_server;
		std::vector<Channel*> _channelList;
	public:
		Client(int fd, Server* server);
		// Client(std::string nickname, std::string username);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &copy);
		
		//Primarily Startup
		int			get_fd() const;
		std::string	get_nickname() const;
		int			set_nickname(std::string nickname);
		std::string	get_username() const;
		int			set_username(std::string username);
		Server*		get_server() const;

		//User Actions
		int			join_channel(std::string channelName, std::string password);
		int			leave_channel(std::string channelName);
		std::vector<Channel*> get_channelList(void);
		Channel*	get_channel(std::string channelName);
		bool		is_in_channel(std::string channelName) const;
};

#endif
