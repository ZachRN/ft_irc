#include "server.hpp"

Server::Server(std::string setpass, int setport)
{
	pass = setpass;
	port = setport;
}

Server::~Server()
{
	return ;
}

std::string	Server::get_name() const
{
	return (name);
}

std::string	Server::get_pass() const
{
	return (pass);
}

int	Server::get_port() const
{
	return (port);
}