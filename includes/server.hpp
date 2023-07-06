#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
/*
	Private Variables are pending to be added
	as we see fit, possibly things such as timeout response
	etc etc.
*/
class Server
{
	private:
		std::string _name;
		std::string _pass;
		int			_port;
	public:
		Server(std::string pass, int port);
		~Server();
		
		std::string	get_name() const;
		std::string	get_pass() const;
		int			get_port() const;

};

#endif
