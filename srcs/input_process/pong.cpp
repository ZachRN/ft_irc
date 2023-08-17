#include <ctime>
#include "client.hpp"

int pong(Client* client)
{
	std::time_t result = std::time(nullptr);
	client->set_lastPong(result);
	return (SUCCESS);
}
