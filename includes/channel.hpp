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

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::vector<Client *>	_clients;
		std::vector<Client *>	_operators;
	public:
		Channel(std::string name, Client *creator);
		Channel(const Channel &copy);
		~Channel();
		Channel	&operator=(const Channel &copy);
		
		std::string				get_name() const;
		std::string				get_topic() const;
		std::vector<Client *>	get_clients() const;
		std::vector<Client *>	get_operators() const;

		void	set_name(std::string name);
		void	set_topic(std::string topic);
		bool	add_client(Client *client);
		bool	remove_client(Client *client);
		bool	add_operator(Client *client);
		bool	remove_operator(Client *client);
};

#endif
