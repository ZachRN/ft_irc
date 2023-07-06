#include <iostream>
#include "server.hpp"

Server::Server(std::string setpass, int setport)
{
	_pass = setpass;
	_port = setport;
}

Server::~Server()
{
	return ;
}

std::string	Server::get_name() const
{
	return (_name);
}

std::string	Server::get_pass() const
{
	return (_pass);
}

int	Server::get_port() const
{
	return (_port);
}