#ifndef RUN_SERVER_HPP
# define RUN_SERVER_HPP

# include "server.hpp"

int run_server(Server server);
int send_msg(int sockfd, std::string msg);

#endif
