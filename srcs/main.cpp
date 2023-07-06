#include <iostream>
#include "server.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Arguements should be as follows\n./ircserv [password] [port]" << std::endl;
		return (1);
	}
	(void)argv;
	Server ircServer("testPass", 4242);
	std::cout << "password: " << ircServer.get_pass() << "\nport: " << ircServer.get_port() << std::endl;
}