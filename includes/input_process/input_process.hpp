#ifndef INPUT_PROCESS_HPP
# define INPUT_PROCESS_HPP

#include "server.hpp"
#include "jump.hpp"
#include "nickname.hpp"

int	input_process(int fd, char buffer[1024], Server *server);

#endif