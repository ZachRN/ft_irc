#include <iostream>
#include "run_server.hpp"
#include "server.hpp"

//Hard defining instead of making a .h
// void	run_tests(void);
void	client_channel_tests(void);

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Arguements should be as follows\n./ircserv [password] [port]" << std::endl;
		return (1);
	}
	(void)argv;
	Server ircServer("testPass", 4242);
	ircServer.set_maxClients(4096);
	std::cout << "password: " << ircServer.get_pass() << "\nport: " << ircServer.get_port() << std::endl;
	ircServer.add_client("zach", "zach");
	run_server(ircServer);
	//run_tests();
}