#include <iostream>
#include <fstream>
#include <unordered_map>
#include "config.hpp"
#include "utils.hpp"

Config::Config()
{
	_host = "default";
	_serverName = "default";
	_version = "0.0";
	_userModes = "";
	_channelModes = "";
	_motd = "Welcome to the Internet Relay Chat server!\n";
	_timeout = 60;
	_maxClients = 4096;
}

Config::~Config()
{
}

std::string	Config::get_host() const
{
	return (_host);
}

std::string	Config::get_serverName() const
{
	return (_serverName);
}

std::string	Config::get_version() const
{
	return (_version);
}

std::string	Config::get_userModes() const
{
	return (_userModes);
}

std::string	Config::get_channelModes() const
{
	return (_channelModes);
}

std::string	Config::get_motd() const
{
	return (_motd);
}

int			Config::get_timeout() const
{
	return (_timeout);
}

int			Config::get_maxClients() const
{
	return (_maxClients);
}

void		Config::print_config() const
{
	std::cout << "host: " << _host << std::endl;
	std::cout << "serverName: " << _serverName << std::endl;
	std::cout << "version: " << _version << std::endl;
	std::cout << "userModes: " << _userModes << std::endl;
	std::cout << "channelModes: " << _channelModes << std::endl;
	std::cout << "motd: " << _motd << std::endl;
	std::cout << "timeout: " << _timeout << std::endl;
	std::cout << "maxClients: " << _maxClients << std::endl;
}

std::unordered_map<std::string, std::string> read_config(std::string configPath)
{
	std::unordered_map<std::string, std::string> configData;
	std::ifstream configFile(configPath);
	if (!configFile.is_open())
	{
		throw std::exception();
	}
	std::string line;
	while (std::getline(configFile, line))
	{
		// Skip lines starting with "//"
		if (line.find("//") == 0)
		{
			continue;
		}

		size_t delimiterPos = line.find("=");
		if (delimiterPos != std::string::npos)
		{
			std::string key = trim_st(line.substr(0, delimiterPos));
			std::string value = trim_st(line.substr(delimiterPos + 1));
			configData[key] = value;
		}
	}

	configFile.close();
	return (configData);
}

int Config::parse_config(std::string configPath)
{
	std::unordered_map<std::string, std::string> configData;
	try
	{
		configData = read_config(configPath);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing config file." << std::endl;
		return (FAILURE);
	}

	// Print the parsed config data
	for (std::unordered_map<std::string, std::string>::const_iterator i = configData.begin(); i != configData.end(); ++i)
	{
		std::cout << "<" << i->first << "><" << i->second << ">" << std::endl;
	}

	return (SUCCESS);
}
