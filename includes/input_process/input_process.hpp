#ifndef INPUT_PROCESS_HPP
# define INPUT_PROCESS_HPP

#include "server.hpp"
#include "join.hpp"
#include "nickname.hpp"
#include "username.hpp"
#include "leave.hpp"
#include "privmsg.hpp"
#include "mode.hpp"

int	input_process(int fd, char buffer[1024], Server *server);

#endif