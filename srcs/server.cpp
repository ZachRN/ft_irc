#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility> // std::make_pair
#include "client.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "leave.hpp"
#include "server.hpp"
#include "utils.hpp"

//Initilization
Server::Server(std::string setpass, int setport)
{
	// This probably also needs some type of validation in order to correctly
	// Checking if the port is a valid number, as well as the syntax of the password
	_pass = setpass;
	_port = setport;
}

Server::~Server()
{
	return ;
}
//End of Intilization

//Regular Variable Functions
// std::string	Server::get_name() const
// {
// 	return (_name);
// }

Config	Server::get_config() const
{
	return (_config);
}

std::string	Server::get_pass() const
{
	return (_pass);
}

int	Server::get_port() const
{
	return (_port);
}

int	Server::get_nfds() const
{
	return (_nfds);
}

void	Server::set_nfds(int nfds)
{
	_nfds = nfds;
}

void	Server::increment_nfds()
{
	++_nfds;
}

void	Server::decrement_nfds()
{
	--_nfds;
}
//End of Regular Variable Functions

//Client Map Functions
std::map<int, Client>* Server::get_clientList()
{
	return (&_clientList);
}


//current error solution is to return nullptr instead of throw.
Client*	Server::get_client(int fd)
{
	std::map<int, Client>::iterator client = _clientList.find(fd);
	if (client == _clientList.end())
		return (nullptr);
	return (&client->second);
}

Client* Server::get_client(std::string nickname)
{
	for (std::map<int, Client>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
	{
		if (it->second.get_nickname() == nickname)
			return (&it->second);
	}
	return (nullptr);
}

//Current solution is to use enums instead of throwing for explicit error messages
//this is primarily to save on resources
int	Server::add_client(int fd)
{
	if (this->get_client(fd) != nullptr)
		return (FD_IN_USE);
	// if (!name_syntax_check(nickname) || !name_syntax_check(username))
	// 	return (NAME_SYNTAX_INVALID);
	Client client(fd, this);
	_clientList.insert(std::make_pair(fd, client));
	return (SUCCESS);
}

int	Server::remove_client(int fd)
{
	std::map<int, Client>::iterator client = _clientList.find(fd);
	if (client == _clientList.end())
		return (NO_CLIENT_FOUND);
	//Remove Client From Every Channel
	std::vector<Channel*> channelList = client->second.get_channelList();
	for (std::vector<Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		std::string channelName = (*it)->get_name();
		client->second.leave_channel((*it)->get_name());
		// If channel still exists, send a PART message to all clients in the channel
		if (get_channel(channelName) != nullptr)
		{
			std::string message = (":" + client->second.get_fullref() + " PART #" + channelName + "\n");
			get_channel(channelName)->send_all_message(message);
		}
	}
	_clientList.erase(client);
	return (SUCCESS);
}

bool	Server::nickname_in_use(std::string nickname)
{
	if (get_client(nickname) == nullptr)
		return (false);
	return (true);
}
//End of Client Map Functions

//Stat of Channel Map Functions

std::map<std::string, Channel>* Server::get_channelList()
{
	return (&_channelList);
}

Channel*	Server::get_channel(std::string channelName)
{
	std::map<std::string, Channel>::iterator channel = _channelList.find(channelName);
	if (channel == _channelList.end())
		return (nullptr);
	return (&channel->second);
}

int	Server::add_channel(std::string channelName, Client &client)
{
	if (get_channel(channelName) != nullptr)
		return (CHANNEL_NAME_IN_USE);
	if (name_syntax_check(channelName) != SUCCESS)
		return (NAME_SYNTAX_INVALID);
	Channel channel(channelName, &client, this);
	_channelList.insert(std::make_pair(channelName, channel));
	return (SUCCESS);
}

int	Server::remove_channel(std::string channelName)
{
	std::map<std::string, Channel>::iterator channel = _channelList.find(channelName);
	if (channel == _channelList.end())
		return (NO_CHANNEL_FOUND);
	_channelList.erase(channel);
	return (SUCCESS);
}

//END OF CHANNEL MAP FUCNTIONS

//Socket Functions
int	Server::get_socket() const
{
	return (_socket);
}

int	Server::init_socket()
{
	// Create a socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
	{
		std::cerr << "Error creating socket." << std::endl;
		return (FAILURE);
	}

	// Set the socket to non-blocking mode
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	// Bind the socket to the designated port
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	if (bind(_socket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) == -1)
	{
		std::cerr << "Error binding socket to port " << _port << std::endl;
		close(_socket);
		return (FAILURE);
	}
	return (SUCCESS);
}
//End of Socket Functions
