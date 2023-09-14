#include <iostream>

#include "server.hpp"
#include "utils.hpp"

//Hard defining instead of making a .h
// void	run_tests(void);
void	client_channel_tests(void);

static std::string server_pass(std::string pass)
{
	if (name_syntax_check(pass) != SUCCESS)
	{
		std::cout << "Password did not match syntax 0-9a-zA-Z _-" << std::endl;
		exit(FAILURE);
	}
	return (pass);
}

static int server_port(std::string port)
{
	int numPort = -1;
	try{
		numPort = std::stoi(port);
	}
	catch (...){
		std::cout << "Port was not a valid integer." << std::endl;
		exit(FAILURE);
	}
	if (numPort < 1 || numPort > 65535)
	{
		std::cout << "The port must range from 0 to 65535" << std::endl;
		exit(FAILURE);
	}
	return (numPort);
}

int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Arguments should be as follows\n./ircserv [password] [port]" << std::endl;
		return (1);
	}
	client_channel_tests();
	Server ircServer(server_pass(argv[1]), server_port(argv[2]));
	ircServer.get_config().parse_config("configs/config.txt");
	std::cout << "password: " << ircServer.get_pass() << "\nport: " << ircServer.get_port() << std::endl;
	// run_server(&ircServer);
	ircServer.start_server();
	return (SUCCESS);
}
