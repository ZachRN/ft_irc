#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include "server.hpp"

int privmsg(Client* client, std::vector<std::string> parsed_input, Server *server);

#endif