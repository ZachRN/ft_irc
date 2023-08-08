#ifndef LEAVE_HPP
# define LEAVE_HPP

#include "server.hpp"

void send_leave(Client* client, Channel* channel, std::string channel_name);
int leave(Client* client, std::vector<std::string> parsed_input, Server *server);

#endif