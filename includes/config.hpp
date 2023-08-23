#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>

class Config
{
	private:
		std::string	_host;
		std::string	_serverName;
		std::string	_version;
		std::string	_userModes;
		std::string	_channelModes;
		std::string	_motd;
		int			_timeout;
		size_t		_maxClients;
	public:
		Config();
		~Config();
		std::string	get_host() const;
		std::string	get_serverName() const;
		std::string	get_version() const;
		std::string	get_userModes() const;
		std::string	get_channelModes() const;
		std::string	get_motd() const;
		int			get_timeout() const;
		size_t		get_maxClients() const;

		void		print_config() const;
		int			parse_config(std::string configPath);
};

#endif
