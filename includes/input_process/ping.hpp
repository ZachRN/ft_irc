#ifndef PING_HPP
# define PING_HPP

# include "server.hpp"

int ping(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed);

#endif
