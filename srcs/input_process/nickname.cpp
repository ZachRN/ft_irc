#include <map>
#include "server.hpp"
#include "run_server.hpp"

static void send_nick(Client* client, Channel* channel, std::string channel_name)
{
	std::vector<Client*> clientList = channel->get_clients();
	std::string message = client->get_fullref();
	message.append(":zach!gmail JOIN ");
	message.append(channel_name);
	message.append("\n");
	for (std::vector<Client*>::const_iterator to_send = clientList.begin(); to_send != clientList.end(); to_send++)
	{
		send_msg((*to_send)->get_fd(), message);
	}
}

static int	new_nick(std::vector<std::string> parsed_input)
{

}

int nickname(Client* client, std::vector<std::string> parsed_input, Server *server)
{

	// std::string channel_name = (*(++(parsed_input.begin()))).substr(1);
	if (client->get_nickname() == "")
		return (new_nick(parsed_input));
	if (parsed_input.size() > 2)
		return (FAILURE);
	// send_join(client, channel, (*(++(parsed_input.begin()))));
	return (SUCCESS);
}


// [][][][][]
// [ab][ac][ad][ag]
// [ac]
// [c...]