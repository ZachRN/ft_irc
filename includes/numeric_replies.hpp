#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

/*
	These are the numeric replies that are used to indicate to the client
	how the server is responding to the client's commands.
*/
enum error_replies
{
	ERR_NOSUCHNICK = 401, // "<nickname> :No such nick/channel" - Used to indicate the nickname parameter supplied to a command is currently unused.
	ERR_NOSUCHSERVER = 402, // "<server name> :No such server" - Used to indicate the server name given currently doesn't exist.
	ERR_NOSUCHCHANNEL = 403, // "<channel name> :No such channel" - Used to indicate the given channel name is invalid.
	ERR_CANNOTSENDTOCHAN = 404, // "<channel name> :Cannot send to channel" - Sent to a user who is either (a) not on a channel which is mode +n or (b) not a chanop (or mode +v) on a channel which has mode +m set and is trying to send a PRIVMSG message to that channel.
	ERR_TOOMANYCHANNELS = 405, // "<channel name> :You have joined too many channels" - Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
	ERR_WASNOSUCHNICK = 406, // "<nickname> :There was no such nickname" - Returned by WHOWAS to indicate there is no history information for that nickname.
	ERR_TOOMANYTARGETS = 407, // "<target> :<error code> recipients. <abort message>" - Returned to a client which is attempting to send a PRIVMSG/NOTICE using the user\@host destination format and for a user\@host which has several occurrences.
	ERR_NOORIGIN = 409, // ":No origin specified" - PING or PONG message missing the originator parameter which is required since these commands must work without valid prefixes.
	ERR_NORECIPIENT = 411, // ":No recipient given (<command>)" - Returned when no recipient is given with a command.
	ERR_NOTEXTTOSEND = 412, // ":No text to send" - Returned when NOTICE/PRIVMSG is used with no message given.
	ERR_NOTOPLEVEL = 413, // "<mask> :No toplevel domain specified" - Used when a message is being sent to a mask without being limited to a top-level domain (i.e. * instead of *.au).
	ERR_WILDTOPLEVEL = 414, // "<mask> :Wildcard in toplevel domain" - Used when a message is being sent to a mask with a wild-card for a top level domain (i.e. *.*).
	ERR_UNKNOWNCOMMAND = 421, // "<command> :Unknown command" - Returned when the given command is unknown to the server (or hidden because of lack of access rights).
	ERR_NOMOTD = 422, // ":MOTD File is missing" - Server's MOTD file could not be opened by the server.
	ERR_NOADMININFO = 423, // "<server> :No administrative info available" - Returned by a server in response to an ADMIN message when there is an error in finding the appropriate information.
	ERR_FILEERROR = 424, // ":File error doing <file op> on <file>" - Generic error message used to report a failed file operation during the processing of a message.
	ERR_NONICKNAMEGIVEN = 431, // ":No nickname given" - Returned when a nickname parameter expected for a command and isn't found.
	ERR_ERRONEUSNICKNAME = 432, // "<nick> :Erroneous nickname" - Returned after receiving a NICK message which contains characters which do not fall in the defined set. See section x.x.x for details on valid nicknames.
	ERR_NICKNAMEINUSE = 433, // "<nick> :Nickname is already in use" - Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
	ERR_NICKCOLLISION = 436, // "<nick> :Nickname collision KILL" - Returned by a server to a client when it detects a nickname collision (registered of a NICK that already exists by another server).
	ERR_USERNOTINCHANNEL = 441, // "<nick> <channel> :They aren't on that channel" - Returned by the server to indicate that the target user of the command is not on the given channel.
	ERR_NOTONCHANNEL = 442, // "<channel> :You're not on that channel" - Returned by the server whenever a client tries to perform a channel affecting command for which the client isn't a member.
	ERR_USERONCHANNEL = 443, // "<user> <channel> :is already on channel" - Returned when a client tries to invite a user to a channel they are already on.
	ERR_NOLOGIN = 444, // "<user> :User not logged in" - Returned by the summon after a SUMMON command for a user was unable to be performed since they were not logged in.
	ERR_SUMMONDISABLED = 445, // ":SUMMON has been disabled" - Returned as a response to the SUMMON command. Must be returned by any server which doesn't implement it.
	ERR_USERSDISABLED = 446, // ":USERS has been disabled" - Returned as a response to the USERS command. Must be returned by any server which does not implement it.
	ERR_NOTREGISTERED = 451, // ":You have not registered" - Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
	ERR_NEEDMOREPARAMS = 461, // "<command> :Not enough parameters" - Returned by the server by numerous commands to indicate to the client that it didn't supply enough parameters.
	ERR_ALREADYREGISTRED = 462, // ":Unauthorized command (already registered)" - Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message).
	ERR_NOPERMFORHOST = 463, // ":Your host isn't among the privileged" - Returned to a client which attempts to register with a server which does not been setup to allow connections from the host the attempted connection is tried.
	ERR_PASSWDMISMATCH = 464, // ":Password incorrect" - Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
	ERR_YOUREBANNEDCREEP = 465, // ":You are banned from this server" - Returned after an attempt to connect and register yourself with a server which has been setup to explicitly deny connections to you.
	ERR_KEYSET = 467, // "<channel> :Channel key already set" - Any command requiring special privileges must return this error to indicate the attempt was unsuccessful.
	ERR_CHANNELISFULL = 471, // "<channel> :Cannot join channel (+l)" - Returned when a JOIN message is sent to a channel that has been set +l and user trying to join is not a chanop (or mode +v) on that channel.
	ERR_UNKNOWNMODE = 472, // "<char> :is unknown mode char to me" - Returned when a client tries to set a mode char that is unknown to the server.
	ERR_INVITEONLYCHAN = 473, // "<channel> :Cannot join channel (+i)" - Returned when a given channel is invite-only and the user was not invited to it.
	ERR_BANNEDFROMCHAN = 474, // "<channel> :Cannot join channel (+b)" - Returned when a client tries to join a channel that is protected by the channel key and the key they gave is incorrect.
	ERR_BADCHANNELKEY = 475, // "<channel> :Cannot join channel (+k)" - Returned when a client tries to join a channel that is protected by the channel key and the key they gave is incorrect.
	ERR_NOPRIVILEGES = 481, // ":Permission Denied- You're not an IRC operator" - Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
	ERR_CHANOPRIVSNEEDED = 482, // "<channel> :You're not channel operator" - Any command requiring 'chanop' privileges (such as MODE messages) must return this error if the client making the attempt is not a chanop on the specified channel.
	ERR_CANTKILLSERVER = 483, // ":You cant kill a server!" - Any attempts to use the KILL command on a server are to be refused and this error returned directly to the client.
	ERR_NOOPERHOST = 491, // ":No O-lines for your host" - If a client sends an OPER message and the server has not been configured to allow connections from the client's host as an operator, this error must be returned.
	ERR_UMODEUNKNOWNFLAG = 501, // ":Unknown MODE flag" - Returned by the server to indicate that a MODE message was sent with a nickname parameter and that the a mode flag sent was not recognized.
	ERR_USERSDONTMATCH = 502 // ":Cant change mode for other users" - Error sent to any user trying to view or change the user mode for a user other than themselves.
};

/*
	These are the numeric replies that are used to indicate to the client
	how the server is responding to the client's commands.
*/
enum command_replies
{
	RPL_WELCOME = 1, // "Welcome to the Internet Relay Network <nickname>" - Sent after a client registers with the server (numerics 001 to 004 must be sent before this is sent).
	RPL_YOURHOST = 2, // "Your host is <servername>, running version <ver>" - Part of the post-registration greeting. Text varies widely.
	RPL_CREATED = 3, // "This server was created <date>" - Part of the post-registration greeting. Text varies widely.
	RPL_MYINFO = 4, // "<servername> <version> <available user modes> <available channel modes>" - Part of the post-registration greeting. Text varies widely.
	RPL_BOUNCE = 5, // "Try server <server name>, port <port number>" - Sent by the server to a user to suggest an alternative server. This is often used when the connection is refused because the server is already full.
	RPL_NONE = 300, // Dummy reply number. Not used.
	RPL_USERHOST = 302, // ":*1<reply> *( " " <reply> )" - Reply format used by USERHOST to list replies to the query list. The reply string is composed as follows: reply = nickname [ "*" ] "=" ( "+" / "-" ) hostname
	RPL_ISON = 303, // ":*1<nick> *( " " <nick> )" - Reply format used by ISON to list replies to the query list.
	RPL_AWAY = 301, // "<nick> :<away message>" - Used in reply to a command directed at a user who is marked as away.
	RPL_UNAWAY = 305, // ":You are no longer marked as being away" - Sent to any client which is successfully removing an AWAY message.
	RPL_NOWAWAY = 306, // ":You have been marked as being away" - Sent to any client which is successfully setting an AWAY message.
	RPL_WHOISUSER = 311, // "<nick> <user> <host> * :<real name>" - Reply to WHOIS - Information about the user.
	RPL_WHOISSERVER = 312, // "<nick> <server> :<server info>" - Reply to WHOIS - What server they're on.
	RPL_WHOISOPERATOR = 313, // "<nick> :is an IRC operator" - Reply to WHOIS - User has IRC operator privileges.
	RPL_WHOISIDLE = 317, // "<nick> <integer> :seconds idle" - Reply to WHOIS - Idle information.
	RPL_ENDOFWHOIS = 318, // "<nick> :End of WHOIS list" - Reply to WHOIS - End of list.
	RPL_WHOISCHANNELS = 319, // "<nick> :*( ( "@" / "+" ) <channel> " " )" - Reply to WHOIS - Channel list for user.
	RPL_WHOWASUSER = 314, // "<nick> <user> <host> * :<real name>" - Reply to WHOWAS - Information about the user.
	RPL_ENDOFWHOWAS = 369, // "<nick> :End of WHOWAS" - Reply to WHOWAS - End of list.
	RPL_LISTSTART = 321, // "Channel :Users Name" - Start of an RPL_LIST - Replies.
	RPL_LIST = 322, // "<channel> <# visible> :<topic>" - RPL_LIST - Replies. channel <space> # visible <space> topic
	RPL_LISTEND = 323, // ":End of LIST" - Termination of an RPL_LIST - list.
	RPL_CHANNELMODEIS = 324, // "<channel> <mode> <mode params>" - Reply to MODE - Returns the channel modes.
	RPL_NOTOPIC = 331, // "<channel> :No topic is set" - When sending a TOPIC message to determine the channel topic, one of two replies is sent. If the topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC.
	RPL_TOPIC = 332, // "<channel> :<topic>" - When sending a TOPIC message to determine the channel topic, one of two replies is sent. If the topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC.
	RPL_INVITING = 341, // "<channel> <nick>" - Returned by the server to indicate that the attempted INVITE message was successful and is being passed onto the end client.
	RPL_SUMMONING = 342, // "<user> :Summoning user to IRC" - Returned by a server answering a SUMMON message to indicate that it is summoning that user.
	RPL_VERSION = 351, // "<version>.<debuglevel> <server> :<comments>" - Reply by the server showing its version details. The <version> is the version of the software being used (including any patchlevel revisions) and the <debuglevel> is used to indicate if the server is running in "debug mode".
	RPL_WHOREPLY = 352, // "<channel> <user> <host> <server> <nick> ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] :<hopcount> <real name>" - Reply to WHO - Information about the user. channel <space> user <space> host <space> server <space> nick ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] : hopcount <space> real name
	RPL_ENDOFWHO = 315, // "<name> :End of WHO list" - Reply to WHO - End of list.
	RPL_NAMREPLY = 353, // "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )" - Reply to NAMES - List of users in a channel. channel <space> ( "=" / "*" / "@" ) [ "@" / "+" ] : nick *( " " [ "@" / "+" ] nick )
	RPL_ENDOFNAMES = 366, // "<channel> :End of NAMES list" - Reply to NAMES - End of list.
	RPL_LINKS = 364, // "<mask> <server> :<hopcount> <server info>" - Reply to LINKS - List of servers and their information. mask <space> server <space> hopcount <space> server info
	RPL_ENDOFLINKS = 365, // "<mask> :End of LINKS list" - Termination of an RPL_LINKS list.
	RPL_BANLIST = 367, // "<channel> <banid>" - A ban-list item. channel <space> banid
	RPL_ENDOFBANLIST = 368, // "<channel> :End of channel ban-list" - Termination of an RPL_BANLIST list.
	RPL_INFO = 371, // ":<string>" - A server specific msg re MOTD etc.
	RPL_ENDOFINFO = 374, // ":End of INFO list" - Termination of an RPL_INFO list.
	RPL_MOTDSTART = 375, // ":- <server> Message of the day - " - Start of an RPL_MOTD list.
	RPL_MOTD = 372, // ":- <text>" - Server MOTD text.
	RPL_ENDOFMOTD = 376, // ":End of MOTD command" - Termination of an RPL_MOTD list.
	RPL_YOUREOPER = 381, // ":You are now an IRC operator" - Successful reply from OPER.
	RPL_REHASHING = 382, // "<config file> :Rehashing" - Successful reply from REHASH.
	RPL_TIME = 391, // "<server> :<string showing server's local time>" - Reply to TIME.
	RPL_USERSSTART = 392, // ":UserID Terminal Host" - Start of an RPL_USERS list.
	RPL_USERS = 393, // ":%-8s %-9s %-8s" - Reply to USERS.
	RPL_ENDOFUSERS = 394, // ":End of users" - Termination of an RPL_USERS list.
	RPL_NOUSERS = 395, // ":Nobody logged in" - Reply to USERS when nobody is logged in.
	RPL_TRACELINK = 200, // "Link <version & debug level> <destination> <next server> V<protocol version> <link uptime in seconds> <backstream sendq> <upstream sendq>" - Reply by the server showing its version details. The <version> is the version of the software being used (including any patchlevel revisions) and the <debuglevel> is used to indicate if the server is running in "debug mode".
	RPL_TRACECONNECTING = 201, // "Try. <class> <server>" - Reply by the server when tracing a connection.
	RPL_TRACEHANDSHAKE = 202, // "H.S. <class> <server>" - Reply by the server when tracing a connection.
	RPL_TRACEUNKNOWN = 203, // "???? <class> [<client IP address in dot form>]" - Reply by the server when tracing a connection.
	RPL_TRACEOPERATOR = 204, // "Oper <class> <nick>" - Reply by the server when tracing a connection.
	RPL_TRACEUSER = 205, // "User <class> <nick>" - Reply by the server when tracing a connection.
	RPL_TRACESERVER = 206, // "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server>" - Reply by the server when tracing a connection.
	RPL_TRACENEWTYPE = 208, // "<newtype> 0 <client name>" - Reply by the server to a TRACE message which is either malformed or which cannot be answered for some other reason.
	RPL_TRACELOG = 261, // "File <logfile> <debug level>" - Reply by the server containing the trace log file for the server replying to TRACE message.
	RPL_STATSLINKINFO = 211, // "<linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>" - Reply to STATS - Link information.
	RPL_STATSCOMMANDS = 212, // "<command> <count>" - Reply to STATS - Commands run.
	RPL_STATSCLINE = 213, // "C <host> * <name> <port> <class>" - Reply to STATS - C line info.
	RPL_STATSNLINE = 214, // "N <host> * <name> <port> <class>" - Reply to STATS - N line info.
	RPL_STATSILINE = 215, // "I <host> * <host> <port> <class>" - Reply to STATS - I line info.
	RPL_STATSKLINE = 216, // "K <host> * <username> <port> <class>" - Reply to STATS - K line info.
	RPL_STATSYLINE = 218, // "Y <class> <ping frequency> <connect frequency> <max sendq>" - Reply to STATS - Y line info.
	RPL_ENDOFSTATS = 219, // "<stats letter> :End of STATS report" - Reply to STATS when all statistics are listed.
	RPL_STATSLLINE = 241, // "L <hostmask> * <servername> <maxdepth>" - Reply to STATS - L line info.
	RPL_STATSUPTIME = 242, // ":Server Up %d days %d:%02d:%02d" - Reply to STATS - Uptime info.
	RPL_STATSOLINE = 243, // "O <hostmask> * <name>" - Reply to STATS - O line info.
	RPL_STATSHLINE = 244, // "H <hostmask> * <servername>" - Reply to STATS - H line info.
	RPL_UMODEIS = 221, // "<user mode string>" - Reply by the server showing its version details. The <version> is the version of the software being used (including any patchlevel revisions) and the <debuglevel> is used to indicate if the server is running in "debug mode".
	RPL_LUSERCLIENT = 251, // ":There are <integer> users and <integer> invisible on <integer> servers" - Reply to LUSERS - Information about local connections; the rest is global.
	RPL_LUSEROP = 252, // "<integer> :operator(s) online" - Reply to LUSERS - Number of IRC operators online.
	RPL_LUSERUNKNOWN = 253, // "<integer> :unknown connection(s)" - Reply to LUSERS - Number of unknown connections.
	RPL_LUSERCHANNELS = 254, // "<integer> :channels formed" - Reply to LUSERS - Number of channels formed.
	RPL_LUSERME = 255, // ":I have <integer> clients and <integer> servers" - Reply to LUSERS - Information about local connections; the rest is global.
	RPL_ADMINME = 256, // "<server> :Administrative info" - Reply to ADMIN - Administrative info about server.
	RPL_ADMINLOC1 = 257, // ":<admin info>" - Reply to ADMIN - Administrative info about server.
	RPL_ADMINLOC2 = 258, // ":<admin info>" - Reply to ADMIN - Administrative info about server.
	RPL_ADMINEMAIL = 259 // ":<admin info>" - Reply to ADMIN - Administrative info about server.
};

/*
	These numerics are not described above since they fall into one of
	the following categories:

	1. no longer in use;

	2. reserved for future planned use;

	3. in current use but are part of a non-generic 'feature' of
		the current IRC server.
*/
enum reserved_numerics
{
	RPL_TRACECLASS = 209,
	RPL_STATSQLINE = 217,
	RPL_SERVICEINFO = 231,
	RPL_ENDOFSERVICES = 232,
	RPL_SERVICE = 233,
	RPL_SERVLIST = 234,
	RPL_SERVLISTEND = 235,
	RPL_WHOISCHANOP = 316,
	RPL_KILLDONE = 361,
	RPL_CLOSING = 362,
	RPL_CLOSEEND = 363,
	RPL_INFOSTART = 373,
	RPL_MYPORTIS = 384,
	ERR_YOUWILLBEBANNED = 466,
	ERR_BADCHANMASK = 476,
	ERR_NOSERVICEHOST = 492
};

/*
	These are custom reply codes that are not part of the RFC.
*/
enum custom_replies
{
	ERR_SERVERISFULL = 999, // ":Server is full"
};

#endif
