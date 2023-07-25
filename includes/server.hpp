#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>

# define SUCCESS 0
# define FAILURE 1

/*
	Private Variables are pending to be added
	as we see fit, possibly things such as timeout response
	etc etc.
*/
class Client;

class Server
{
	private:
		//probably dont need to change or have a server name
		// std::string _name;
		std::string _pass;
		int			_port;
		std::map<int, Client> _clientList;
	public:
		//Initilization
		Server(std::string pass, int port);
		~Server();
		
		// regular variable functions
		// std::string	get_name() const;
		std::string	get_pass() const;
		int			get_port() const;

		//Client Map Functions
		
		//Nickname is for 	some reason what IRC uses as a way
		//to uniquely define people, thus a nickname == username in modern
		std::map<int, Client> get_clientList() const;
		Client*		get_client(int fd);
		Client*		get_client(std::string nickname);
		//Not Sure how we want to handle errors with adding and removing clients
		//Do we want to throw? Right now it is early return, maybe change function to
		//A bool and return true or false depending on pass/fail.
		//Throwing allows more custom errors, not sure if needed. 
		int	add_client(int fd);
		int	remove_client(int fd);
		//This is a wrapper for Client Setnickname to verify 
		int	client_setnickname(int fd, std::string nickname);

};

#endif
