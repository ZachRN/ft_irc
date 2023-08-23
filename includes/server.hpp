#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include "client.hpp"
# include "channel.hpp"
# include "config.hpp"

# define SUCCESS 0
# define FAILURE 1

/*
	Private Variables are pending to be added
	as we see fit, possibly things such as timeout response
	etc etc.
*/
// class Client;
// class Channel;

class Server
{
	private:
		Config		_config;
		std::string	_pass;
		int			_port;
		std::vector<pollfd>	_fds; // File descriptors (clients) currently in use
		int					_nfds; // Number of file descriptors (clients) currently in use
		std::map<int, Client> 			_clientList;
		std::map<std::string, Channel>	_channelList;
		int					_serverSocket;
		struct sockaddr_in	_serverAddr;
	public:
		//Initilization
		Server(std::string pass, int port);
		~Server();
		
		// regular variable functions
		Config		get_config() const;
		// std::string	get_name() const;
		std::string	get_pass() const;
		int			get_port() const;
		int			get_nfds() const;
		void		set_nfds(int nfds);
		void		increment_nfds();
		void		decrement_nfds();

		//Client Map Functions
		
		//Nickname is for 	some reason what IRC uses as a way
		//to uniquely define people, thus a nickname == username in modern
		std::map<int, Client>* get_clientList();
		Client*		get_client(int fd);
		Client*		get_client(std::string nickname);
		//Not Sure how we want to handle errors with adding and removing clients
		//Do we want to throw? Right now it is early return, maybe change function to
		//A bool and return true or false depending on pass/fail.
		//Throwing allows more custom errors, not sure if needed. 
		int	add_client(int fd);
		int	remove_client(int fd);
		//This is a wrapper for Client Setnickname to verify 
		bool	nickname_in_use(std::string nickname);
		//End of Client Map Functions
		
		//Start of Channel Map Functions
		std::map<std::string, Channel>* get_channelList();
		Channel*	get_channel(std::string channelName);
		int	add_channel(std::string channelName, Client &client);
		int	remove_channel(std::string channelName);
		//End of Channel Map Functions

		//Start of Socket Functions
		int		get_socket() const;
		int		init_socket();
		int		close_socket();
		//End of Socket Functions

		//Start of Server Operation Functions
		int		start_server();
		int		init_server();
		void	run_server();
		void	close_server();
		//End of Server Operation Functions

		int		send_msg(int sockfd, std::string msg);
		int		send_all_deques();
};

#endif
