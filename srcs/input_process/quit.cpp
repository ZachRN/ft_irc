#include <iostream>
#include <unistd.h>
#include <vector>
#include "channel.hpp"
#include "client.hpp"
#include "server.hpp"
#include "run_server.hpp"

int	quit(Client* client, Server *server)
{
	for (std::map<int, Client>::const_iterator to_send = server->get_clientList()->begin(); to_send != server->get_clientList()->end(); to_send++)
	{
		if (server->get_client(to_send->first)->get_verified())
			send_msg(to_send->first, ":" + client->get_fullref() + " QUIT :Client Quit\n");
	}
	int fd = client->get_fd();
	server->remove_client(fd);
	close(fd);
	return (SUCCESS);
}
