#include <iostream>
#include <utility> // std::make_pair
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

std::string	Server::get_pass() const
{
	return (_pass);
}

int	Server::get_port() const
{
	return (_port);
}
//End of Regular Variable Functions

//Client Map Functions
std::map<std::string, Client> Server::get_clientList() const
{
	return (_clientList);
}


//current error solution is to return nullptr instead of throw.
Client*	Server::get_client(std::string username)
{
	std::map<std::string, Client>::iterator client = _clientList.find(username);
	if (client == _clientList.end())
		return (nullptr);
	return (&client->second);
}

//Current solution is to use enums instead of throwing for explicit error messages
//this is primarily to save on resources
int	Server::add_client(std::string nickname, std::string username)
{
	if (this->get_client(nickname) != nullptr)
		return (NAME_ALREADY_INUSE);
	if (!name_syntax_check(nickname) || !name_syntax_check(username))
		return (NAME_SYNTAX_INVALID);
	Client client(nickname, username);
	_clientList.insert(std::make_pair(nickname, client));
	return (SUCCESS);
}

int	Server::remove_client(std::string nickname)
{
	std::map<std::string, Client>::iterator client = _clientList.find(nickname);
	if (client == _clientList.end())
		return (NO_CLIENT_FOUND);
	_clientList.erase(client);
	return (SUCCESS);
}
//End of Client Map Functions