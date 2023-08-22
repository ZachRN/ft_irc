#include <iostream>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility> // std::make_pair
#include "client.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "leave.hpp"
#include "run_server.hpp"
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

//END OF CHANNEL MAP FUNCTIONS

//Socket Functions
int	Server::get_socket() const
{
	return (_serverSocket);
}

int	Server::init_socket()
{
	// Create a socket
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		std::cerr << "Error creating socket." << std::endl;
		return (FAILURE);
	}

	// Set the socket to non-blocking mode
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

	// Bind the socket to the designated port
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	if (bind(_serverSocket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) == -1)
	{
		std::cerr << "Error binding socket to port " << _port << std::endl;
		close(_serverSocket);
		return (FAILURE);
	}
	return (SUCCESS);
}
//End of Socket Functions

//Start of Server Operation Functions
int Server::start_server()
{
	if (init_server() == FAILURE)
		return (FAILURE);
	run_server();
	close_server();
	return (SUCCESS);
}

int	Server::init_server()
{
	// Create a socket
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		std::cerr << "Error creating socket." << std::endl;
		return (FAILURE);
	}

	// Set the socket to non-blocking mode
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error setting socket to non-blocking mode." << std::endl;
		close(_serverSocket);
		return (FAILURE);
	}

	// Bind the socket to the designated port
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	if (bind(_serverSocket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) == -1)
	{
		std::cerr << "Error binding socket to port " << _port << std::endl;
		close(_serverSocket);
		return (FAILURE);
	}

	// Listen on the socket
	if (listen(_serverSocket, 1) == -1)
	{
		std::cerr << "Error listening on socket." << std::endl;
		close(_serverSocket);
		return (FAILURE);
	}
	std::cout << "Server listening on port " << _port << std::endl;
	pollfd serverPoll;
	serverPoll.fd = _serverSocket;
	serverPoll.events = POLLIN;
	_fds.push_back(serverPoll);
	return (SUCCESS);
}

void	Server::run_server()
{
	// while (true)
	// {
	// 	int ret = poll(_fds.data(), _fds.size(), -1); // Wait indefinitely

	// 	if (ret == -1)
	// 	{
	// 		std::cerr << "Error polling sockets." << std::endl;
	// 		break;
	// 	}

	// 	for (size_t i = 0; i < _fds.size(); ++i) {
	// 		if (_fds[i].revents & POLLIN)
	// 		{
	// 			if (i == 0)
	// 			{
	// 				// Server socket has incoming connection
	// 				// Handle the new connection and add the client socket to the vector
	// 				struct sockaddr_in clientAddr;
	// 				socklen_t clientAddrSize = sizeof(clientAddr);
	// 				int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
	// 				if (clientSocket == -1)
	// 				{
	// 					std::cerr << "Error accepting connection." << std::endl;
	// 				}
	// 				else if (clientSocket != -1 && _nfds < _config.get_maxClients() + 1)
	// 				{
	// 					_fds[_nfds].fd = clientSocket;
	// 					_fds[_nfds].events = POLLIN;
	// 					++_nfds;
	// 					add_client(clientSocket);
	// 					std::cout << "New client connected #" << _nfds - 1 << "." << std::endl;
	// 				}
	// 				else if (clientSocket != -1)
	// 				{
	// 					std::cout << "Maximum number of clients reached." << std::endl;
	// 					std::string msg = ":" + _config.get_host() + " 999 " + _config.get_serverName() + " :Maximum number of clients reached.\r\n";
	// 					send(clientSocket, msg.c_str(), msg.length(), 0);
	// 					close(clientSocket);
	// 				}
	// 			}
	// 			else
	// 			{
	// 				// Client socket has incoming data
	// 				// Handle the received data
	// 			}
	// 		}

	// 		if (_fds[i].revents & POLLOUT)
	// 		{
	// 			if (i >= 2)
	// 			{
	// 				// Client socket is ready for writing
	// 				// Send data to the client
	// 			}
	// 		}

	// 		if (_fds[i].revents & POLLHUP)
	// 		{
	// 			if (i >= 2)
	// 			{
	// 				// Client socket has been disconnected
	// 				// Clean up and remove the socket from the vector
	// 			}
	// 		}
	// 	}
	// }
}

void	Server::close_server()
{
	// Send messages to all clients that the server is shutting down and then close all client sockets
	for (std::map<int, Client>::iterator it = _clientList.begin(); it != _clientList.end(); ++it)
	{
		std::string message = (":" + _config.get_serverName() + " QUIT :Server shutting down.\n");
		send_msg(it->first, message);
		close(it->first);
	}
	// Close the server socket
	close(_serverSocket);
}
//End of Server Operation Functions
