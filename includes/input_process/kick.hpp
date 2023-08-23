#ifndef KICK_HPP
# define KICK_HPP

#include "server.hpp"

int kick(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed);

#endif