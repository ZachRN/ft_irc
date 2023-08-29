#include "server.hpp"

int	password(Client* client, std::vector<std::string> parsed)
{
	return (client->set_passCorrect(parsed.at(1)));
}