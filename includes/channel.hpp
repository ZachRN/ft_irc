#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include "client.hpp"
/*
	1.3 Channels

	A channel is a named group of one or more clients which will all
	receive messages addressed to that channel.  The channel is created
	implicitly when the first client joins it, and the channel ceases to
	exist when the last client leaves it.  While channel exists, any
	client can reference the channel using the name of the channel.

	Channels names are strings (beginning with a '&' or '#' character) of
	length up to 200 characters.  Apart from the the requirement that the
	first character being either '&' or '#'; the only restriction on a
	channel name is that it may not contain any spaces (' '), a control G
	(^G or ASCII 7), or a comma (',' which is used as a list item
	separator by the protocol).

	There are two types of channels allowed by this protocol.  One is a
	distributed channel which is known to all the servers that are

	connected to the network. These channels are marked by the first
	character being a only clients on the server where it exists may join
	it.  These are distinguished by a leading '&' character.  On top of
	these two types, there are the various channel modes available to
	alter the characteristics of individual channels.  See section 4.2.3
	(MODE command) for more details on this.

	To create a new channel or become part of an existing channel, a user
	is required to JOIN the channel.  If the channel doesn't exist prior
	to joining, the channel is created and the creating user becomes a
	channel operator.  If the channel already exists, whether or not your
	request to JOIN that channel is honoured depends on the current modes
	of the channel. For example, if the channel is invite-only, (+i),
	then you may only join if invited.  As part of the protocol, a user
	may be a part of several channels at once, but a limit of ten (10)
	channels is recommended as being ample for both experienced and
	novice users.  See section 8.13 for more information on this.

*/

enum ChannelErrors
{
	CHANNEL_NAME_IN_USE = 1,
	NO_CHANNEL_FOUND,
	UNABLE_TO_DEMOTE_OWNER,
	REQUIRED_OWNER,
	REQUIRED_OPERATOR,
	ALREADY_OPERATOR,
	ALREADY_IN_CHANNEL,
	PASSWORD_INVALID,
	USER_LIMIT,
	CANT_KICK_OWNER,
	NOT_IN_CHANNEL,
	NOT_AN_OPERATOR,
	REQUIRED_INVITE,
	TOO_HIGH_OF_LIMIT
};

class Server;

class Channel
{
	private:
		std::string	_name;
		std::string	_topic;
		bool		_topic_operator;
		std::string	_password;
		bool		_invite;
		Server*		_server;
		std::vector<Client*>	_clients;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_invitelist;
		Client*		_owner;
		size_t		_userlimit;
		bool		_limitEnabled;
	public:
		Channel(std::string name, Client* creator, Server *server);
		Channel(const Channel &copy);
		~Channel();
		Channel	&operator=(const Channel &copy);

		//name
		std::string				get_name() const;
		std::string				get_topic() const;
		bool					get_topic_operator() const;
		std::string				get_password() const;
		bool					get_invite() const;
		std::vector<Client*>	get_clients() const;
		bool					client_in_channel(std::string nickname) const;
		std::vector<Client*>	get_operators() const;
		bool					client_is_operator(std::string nickname) const;
		Client*					get_owner() const;
		bool					client_is_owner(std::string nickname)	const;

		bool	add_client(Client* client);
		bool	remove_client(Client* client);
		int		leave_channel(Client* client);

		//OPERATOR ONLY COMMANDS
		int		set_topic(std::string topic, Client* client);
		int		kick(Client* to_kick, Client* kicker);
		int		add_operator(Client* to_promote, Client* promoter);
		int		remove_operator(Client* to_demote, Client* demoter);
		int		invite(Client* invitee, Client* inviter);
		int		remove_invite(Client* invitee, Client* inviter);
		bool	is_invited(std::string nickname);
		int		set_invite(Client* client);
		int		set_password(std::string password, Client* client);
		size_t	get_limit() const;
		bool	get_limit_enabled() const;
		int		set_limit(int limit, Client* client);
		int		set_topic_operator(Client* client);
		int		set_flip_limit_enabled(Client* client);

		// Send message to all clients in channel
		int		send_all_message(std::string message);
};

#endif
