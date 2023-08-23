#ifndef TOPIC_HPP
# define TOPIC_HPP

#include "server.hpp"

int topic(Client* client, std::vector<std::string> parsed_input, Server *server, std::string unparsed);

#endif