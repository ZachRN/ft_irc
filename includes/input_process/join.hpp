#ifndef JOIN_HPP
# define JOIN_HPP

#include "server.hpp"

int join(Client* client, std::vector<std::string> parsed_input, Server *server);

#endif