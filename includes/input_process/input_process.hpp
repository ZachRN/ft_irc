#ifndef INPUT_PROCESS_HPP
# define INPUT_PROCESS_HPP

#include "server.hpp"
#include "join.hpp"
#include "nickname.hpp"
#include "username.hpp"
#include "leave.hpp"
#include "privmsg.hpp"
#include "mode.hpp"
#include "invite.hpp"
#include "kick.hpp"
#include "topic.hpp"
#include "ping.hpp"
#include "pong.hpp"
#include "quit.hpp"
#include "password.hpp"

int	input_process(int fd, char buffer[1024], Server *server);

#endif