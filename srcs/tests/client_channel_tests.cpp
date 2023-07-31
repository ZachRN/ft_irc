#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

//Basic Add Client, Set Nickname, Set Username
int	user_creation_test()
{
	std::cout << "User Create Channel Tests" << std::endl;
	Server ircServ("pass", 123);
	if (ircServ.add_client(4) != SUCCESS)
	{
		std::cout << "ERROR:User Could not be created." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Created Single User" << std::endl;
	Client *client = ircServ.get_client(4);
	if (client->set_nickname("Zach") != SUCCESS)
	{
		std::cout << "ERROR:User Could not set nickname." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to set nickname" << std::endl;
	if (client->set_username("Zach") != SUCCESS)
	{
		std::cout << "ERROR:User Could not set username." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to set username" << std::endl;
	if (client->set_username("") == SUCCESS || client->set_nickname("12312312312312312312312312") == SUCCESS)
	{
		std::cout << "ERROR: NAME SYNTAX NOT WORKING" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: NAME SYNTAX CHECK WORKING" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

//Tests creatign a channel, getting the channel from client, getting the channel from server.
int	user_create_channel(void)
{
	std::cout << "User Create Channel Tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	//Everything above here is correct pending user_creation_test() is correct.
	if (client->join_channel("Zachs_Home", "") != SUCCESS)
	{
		std::cout << "ERROR:Channel Failed to Create." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Created a channel" << std::endl;
	if (client->get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "ERROR:Failed to add channel to client channelList" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Channel exists in client channelList" << std::endl;
	if (ircServ.get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "ERROR:Failed to add channel to server channelList" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Channel exists server channelList" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

//Tests craetign users with same FD, same nickname, same username
int	multiple_user_creation_test()
{
	std::cout << "Multiple User Creation Test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	if (ircServ.add_client(4) == SUCCESS)
	{
		std::cout << "ERROR:added multiple clients with same FD" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Clients can't share the same FD" << std::endl;
	if (ircServ.add_client(5) != SUCCESS)
	{
		std::cout << "ERROR:adding multiple clients with different FD's didn't work." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to create multiple clients" << std::endl;
	Client *clienttwo = ircServ.get_client(5);
	if (clienttwo->set_nickname("Zach") == SUCCESS)
	{
		std::cout << "ERROR:multiple clients ith same nickname" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Nicknames are unique" << std::endl;
	clienttwo->set_nickname("Mike");
	if (clienttwo->set_username("Zach") != SUCCESS)
	{
		std::cout << "ERROR:user names should be non-unique" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Usernames are non-unique" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	multiple_channel_creation_test()
{
	std::cout << "Multiple User channel Creation Test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	client->join_channel("Zachs_Home", "");
	if (client->join_channel("Zachs_Home", "") == SUCCESS)
	{
		std::cout << "ERROR:Joined a Channel you already exist in" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Unable to join a channel you exist in." << std::endl;
	if (client->join_channel("Mikes_Home", "") != SUCCESS)
	{
		std::cout << "ERROR:Unable to join multiple channels" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to join multiple channels" << std::endl;
	if (client->get_channel("Mikes_Home") == nullptr)
	{
		std::cout << "ERROR:Failed to add channel to client channelList" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Added channel to client ChannelList with multiple channels" << std::endl;
	if (ircServ.get_channel("Mikes_Home") == nullptr)
	{
		std::cout << "ERROR:Failed to add channel to server channelList" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Added a channel to server ChannelList with multiple channels" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	multiple_users_channel_test()
{
	std::cout << "Multiple User Joining Channel Test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	client->join_channel("Zachs_Home", "");
	ircServ.add_client(5);
	Client *clienttwo = ircServ.get_client(5);
	clienttwo->set_nickname("Mike");
	clienttwo->set_username("Mike");
	int a = clienttwo->join_channel("Zachs_Home", "");
	if (a != SUCCESS)
	{
		std::cout << a << std::endl;
		std::cout << "ERROR:Second Client unable to join first clients room" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Second Client can join first client channel" << std::endl;
	clienttwo->join_channel("Mikes_Home", "");
	if (client->join_channel("Mikes_Home", "") != SUCCESS)
	{
		std::cout << "ERROR:First Client unable to join the second client room" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:First client can join second clients room" << std::endl;
	if (client->get_channel("Mikes_Home") == nullptr || clienttwo->get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "ERROR:One of the clients didn't save the other clients room properly" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Both channels added to their respective channel clients list after join, not the creation one" << std::endl;
	if (ircServ.get_channel("Mikes_Home") == nullptr || ircServ.get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "ERROR:One of the channels not saved to ircServ ClientList" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Server successfully updated with both channels existing." << std::endl;
	Channel* channel = ircServ.get_channel("Mikes_Home");
	if (channel->client_in_channel("Zach") == false || channel->client_in_channel("Mike") == false)
	{
		std::cout << "ERROR:Channel Missing one of the intended users saved in the life" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Channel has both users existing in its clientList" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}
int	remove_user(void)
{
	std::cout << "Removing Client from Server test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	if (ircServ.remove_client(4) != SUCCESS)
	{
		std::cout << "ERROR:Failed to remove client from server." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to remove client from server" << std::endl;
	if (ircServ.get_client(4) != nullptr)
	{
		std::cout << "ERROR:Failed to actually remove client from server" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Server clientList updated to not exist in server clientList" << std::endl;
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	if (ircServ.remove_client(5) != SUCCESS)
	{
		std::cout << "ERROR:Failed to remove client when multiple exist." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Able to remove individual clients with multiple clients existing from server" << std::endl;
	if (ircServ.get_client(5) != nullptr)
	{
		std::cout << "ERROR:Failed to actually remove client when multiple exist." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Server clientList correctly updated with multiple clients and singular removal." << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int		leave_channel_tests(void)
{//SETUP
	std::cout << "Removing Client from channels test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");


	zach->join_channel("Zachs_Home", "");
	// zach->set_nickname("Zach");
	// std::cout << zach->get_nickname() << std::endl;
	if (zach->leave_channel("Zachs_Home") != SUCCESS)
	{
		std::cout << "ERROR:Unable to leave channel that already exists." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Able to leave channel that already exists." << std::endl;
	if (ircServ.get_channel("Zachs_Home") != nullptr)
	{
		std::cout << "ERROR:Channel still exists within Server Channel" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Channel no longer exists after final person leaves." << std::endl;
	zach->join_channel("Zachs_Home", "");
	mike->join_channel("Zachs_Home", "");
	Channel *channel = ircServ.get_channel("Zachs_Home");
	zach->leave_channel("Zachs_Home");
	if (channel->client_is_owner(mike->get_nickname()) == false)
	{
		std::cout << "ERROR: Failed to make non-operator client owner." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Added non-operator client to owner." << std::endl;
	mike->leave_channel("Zachs_Home");
	zach->join_channel("Mikes_Home", "");
	mike->join_channel("Mikes_Home", "");
	frans->join_channel("Mikes_Home", "");
	patrick->join_channel("Mikes_Home", "");
	channel = ircServ.get_channel("Mikes_Home");
	channel->add_operator(mike, zach);
	zach->leave_channel("Mikes_Home");
	if (channel->client_is_owner(mike->get_nickname()) != true || channel->client_is_operator(mike->get_nickname()) != true)
	{
		std::cout << "ERROR: Failed to make operator owner after owner leaves." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Operator took owner role after owner left." << std::endl;
	mike->leave_channel("Mikes_Home");
	frans->set_nickname("Frans3");
	if (channel->client_is_owner(frans->get_nickname()) != true)
	{
		std::cout << "ERROR: Failed to make client owner after name change and they were last." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Client remained owner after name change and operator left" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	remove_client_auto_leave_channel_tests()
{
	std::cout << "remove_client_auto_leave_channel_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	Client* zach = ircServ.get_client(4);
	Client* mike = ircServ.get_client(5);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	zach->join_channel("one", "");
	zach->join_channel("two", "");
	zach->join_channel("three", "");
	zach->join_channel("four", "");
	mike->join_channel("one", "");
	mike->join_channel("two", "");
	mike->join_channel("three", "");
	mike->join_channel("four", "");
	ircServ.remove_client(4);
	Channel *channelone = ircServ.get_channel("one");
	Channel* channeltwo = ircServ.get_channel("two");
	Channel* channelthree = ircServ.get_channel("three");
	Channel* channelfour = ircServ.get_channel("four");
	if ((channelone->client_is_owner(mike->get_nickname()) && channeltwo->client_is_owner(mike->get_nickname()) && channelthree->client_is_owner(mike->get_nickname()) && channelfour->client_is_owner(mike->get_nickname())))
	{
		std::cout << "ERROR: Client Two did not become owner of all four channels after remove_client(one)." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Client two became owner of all channels owned by one after remove_client(one)." << std::endl;
	if (!(channelone->client_in_channel("Zach") || channeltwo->client_in_channel("Zach") || channelthree->client_in_channel("Zach") || channelfour->client_in_channel("Zach")))
	{
		std::cout << "ERROR: Client one still in one of the channels." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: Client one left all channels upon removal." << std::endl;
	// return (SUCCESS);
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	add_remove_operator()
{
	std::cout << "add_remove_operator" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");
	zach->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	if (channel->add_operator(mike, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO PROMOTE OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO PROMOTE OPEARTOR" << std::endl;
	if (channel->remove_operator(zach, mike) == SUCCESS)
	{
		std::cout << "ERROR: OPERATOR ABLE TO DEMOTE OWNER" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OPERATOR UNABLE TO DEMOTE OWNER" << std::endl;
	if (channel->remove_operator(mike, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO DEMOTE OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO DEMOTE OPERATOR" << std::endl;
	if (channel->add_operator(mike, mike) == SUCCESS)
	{
		std::cout << "ERROR: NON-OPERATOR ABLE TO PROMOTE OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: NON-OPERATOR UNABLE TO PROMOTE OPERATOR" << std::endl;
	channel->add_operator(mike, zach);
	if (channel->add_operator(frans, mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO ADD OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OPERATOR ABLE TO ADD OPERATOR" << std::endl;
	if (channel->remove_operator(frans, mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO REMOVE OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OPERATOR ABLE TO REMOVE OPERATOR" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	kick_tests()
{
	std::cout << "kick_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");
	zach->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	mike->join_channel("Zachs_Home","");
	frans->join_channel("Zachs_Home","");
	if (channel->kick(mike, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO KICK CLIENT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO KICK CLIENT" << std::endl;
	mike->join_channel("Zachs_Home", "");
	channel->add_operator(mike, zach);
	if (channel->kick(mike, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO KICK OPERATOR" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO KICK OPEARTOR" << std::endl;
	if (channel->client_in_channel(mike->get_nickname()) || channel->client_is_operator(mike->get_nickname()) || mike->get_channel("Zachs_Home") != nullptr)
	{
		std::cout << "ERROR: AFTER KICKED USER REMAINED IN CHANNEL OR AS OPERATOR OR MAINTED CHANNEL IN THEIR LIST" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: AFTER KICK, NOT IN CHANNEL AS CLIENT, OPERATOR, OR IN THEIR OWN CHANNEL LIST" << std::endl;
	mike->join_channel("Zachs_Home", "");
	channel->add_operator(mike, zach);
	if (channel->kick(zach, mike) == SUCCESS)
	{
		std::cout << "ERROR: OPERATOR ABLE KICK OWNER" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: OPERATOR UNABLE TO KICK OWNER" << std::endl;
	if (channel->kick(frans, mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO KICK CLIENT" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: OPERATOR ABLE TO KICK CLIENT" << std::endl;
	frans->join_channel("Zachs_Home", "");
	if (channel->kick(mike, frans) == SUCCESS)
	{
		std::cout << "ERROR: CLIENT ABLE TO KICK CLIENTS" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: CLIENT UNABLE TO KICK CLIENTS" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	password_channel_tests()
{
	std::cout << "passord_channel_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");
	zach->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	mike->join_channel("Zachs_Home","");
	if (channel->set_password("abc", zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
	if (frans->join_channel("Zachs_Home","") == SUCCESS)
	{
		std::cout << "ERROR: ABLE TO JOIN CHANNEL WITH INCORRECT PASSWORD" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: UNABLE TO CHANNEL JOIN WITH INCORRECT PASSWORD" << std::endl;
	if (frans->join_channel("Zachs_Home","abc") != SUCCESS)
	{
		std::cout << "ERROR: UNABLE TO JOIN CHANNEL WITH CORRECT PASSWORD" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: ABLE TO CHANNEL JOIN WITH CORRECT PASSWORD" << std::endl;
	if (channel->set_password("abcdefihihaidhaisdhash", zach) == SUCCESS)
	{
		std::cout << "ERROR: ABLE TO CHANGE PASSWORD TO INCORRECT SYNTAX" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: UNABLE TO CHANGE PASSWORD TO INCORRECT SYNTAX" << std::endl;
	if (channel->set_password("", zach) != SUCCESS)
	{
		std::cout << "ERROR: UNABLE TO SET PASSWORD TO DEFAULT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: ABLE TO SET PASSWORD TO DEFAULT" << std::endl;
	channel->add_operator(mike, zach);
	if (channel->set_password("abc", mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OPERATOR ABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
	if (channel->set_password("abc", frans) == SUCCESS)
	{
		std::cout << "ERROR: CLIENT ABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT UNABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
	if (channel->set_password("abc", patrick) == SUCCESS)
	{
		std::cout << "ERROR: CLIENT NOT IN CHANNEL ABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT NOT IN CHANNEL UNABLE TO SET_PASSWORD TO CHANNEL" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	user_limit_tests()
{
	std::cout << "user_limit_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");
	zach->join_channel("Zachs_Home", "");
	mike->join_channel("Zachs_Home", "");
	frans->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	channel->add_operator(mike, zach);
	if (channel->set_limit(1, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO SET USER LIMIT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO SET USER LIMIT" << std::endl;
	if (channel->set_limit(1, mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO SET USER LIMIT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OPERATOR ABLE TO SET USER LIMIT" << std::endl;
	if (channel->set_limit(1, frans) == SUCCESS)
	{
		std::cout << "ERROR: CLIENT ABLE TO SET USER LIMIT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT UNABLE TO SET USER LIMIT" << std::endl;
	if (patrick->join_channel("Zachs_Home", "") == SUCCESS)
	{
		std::cout << "ERROR: CLIENT JOINED PASSED CLIENT LIMIT." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT UNABLE TO JOIN PAST CLIENT LIMIT." << std::endl;
	if (channel->set_limit(50000, zach) == SUCCESS)
	{
		std::cout << "ERROR: ABLE TO SET LIMIT OVER CONFIG SETTING." << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: UNABLE TO SET LIMIT OVER CONFIG SETTING." << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	invite_tests()
{
	std::cout << "INVITE_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	ircServ.add_client(5);
	ircServ.add_client(6);
	ircServ.add_client(7);
	Client *zach = ircServ.get_client(4);
	Client *mike = ircServ.get_client(5);
	Client *frans = ircServ.get_client(6);
	Client *patrick = ircServ.get_client(7);
	zach->set_nickname("Zach");
	mike->set_nickname("Mike");
	frans->set_nickname("Frans");
	patrick->set_nickname("Patrick");
	zach->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	if (channel->set_invite(zach, true) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO SET_INVITE" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO SET_INVITE" << std::endl;
	if (mike->join_channel("Zachs_Home", "") == SUCCESS)
	{
		std::cout << "ERROR: CLIENT ABLE TO JOIN WITHOUT AN INVITE" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT UNABLE TO JOIN WITHOUT AN INVITE" << std::endl;
	if (channel->invite(mike, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO INVITE CLIENT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO INVITE CLIENT" << std::endl;
	if (channel->is_invited(mike->get_nickname()) != true)
	{
		std::cout << "ERROR: CHANNEL INVITE LIST FAILED TO ADD CLIENT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CHANNEL INVITE LIST ADDED CLIENT" << std::endl;
	if (mike->join_channel("Zachs_Home", "") != SUCCESS)
	{
		std::cout << "ERROR: CLIENT UNABLE TO JOIN CHANNEL AFTER INVITED" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: CLIENT ABLE TO JOIN CHANNEL AFTER INVITED" << std::endl;
	if (channel->is_invited(mike->get_nickname()) == true)
	{
		std::cout << "ERROR: CHANNEL INVITE LIST FAILED TO REMOVED CLIENT" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CHANNEL INVITE LIST REMOVED CLIENT" << std::endl;
	channel->add_operator(mike, zach);
	if (channel->invite(frans, mike) != SUCCESS)
	{
		std::cout << "ERROR: OPERATOR UNABLE TO INVITE CLIENT TO CHANNEL" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: OPERATOR ABLE TO INVITE CLIENT TO CHANNEL" << std::endl;
	frans->join_channel("Zachs_Home", "");
	if (channel->invite(patrick, frans) == SUCCESS)
	{
		std::cout << "ERROR: CLIENT ABLE TO INVITE CLIENT TO CHANNEL" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: CLIENT UNABLE TO INVITE CLIENT TO CHANNEL" << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}

int	topic_tests()
{
	std::cout << "INVITE_tests" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *zach = ircServ.get_client(4);
	zach->set_nickname("Zach");
	zach->join_channel("Zachs_Home", "");
	Channel* channel = ircServ.get_channel("Zachs_Home");
	std::string topic = "This is a brand new topic!";
	if (channel->set_topic(topic, zach) != SUCCESS)
	{
		std::cout << "ERROR: OWNER UNABLE TO SET_TOPIC" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS: OWNER ABLE TO SET_TOPIC" << std::endl;
	if (channel->get_topic() != topic)
	{
		std::cout << "ERROR: TOPIC NOT EQUAL TO TOPIC JUST SET" << std::endl;
		return (FAILURE);
	}
		std::cout << "SUCCESS: TOPIC SET CORRECTLY." << std::endl;
	std::cout << "\n----------------" << std::endl;
	return (SUCCESS);
}


void	client_channel_tests(void)
{
	if (user_creation_test() != SUCCESS)
		return ;
	if (user_create_channel() != SUCCESS)
		return ;
	if (multiple_user_creation_test() != SUCCESS)
		return ;
	if (multiple_channel_creation_test() != SUCCESS)
		return ;
	if (multiple_users_channel_test() != SUCCESS)
		return ;
	if (remove_user() != SUCCESS)
		return ;
	if (leave_channel_tests() != SUCCESS)
		return ;
	if (remove_client_auto_leave_channel_tests() != SUCCESS)
		return ;
	if (add_remove_operator() != SUCCESS)
		return ;
	if (kick_tests() != SUCCESS)
		return ;
	if (password_channel_tests() != SUCCESS)
		return ;
	if (user_limit_tests() != SUCCESS)
		return ;
	if (invite_tests() != SUCCESS)
		return ;
	if (topic_tests() != SUCCESS)
		return ;
	std::cout << "ALL TESTS PASSED SUCCESSFULLY" << std::endl;
	// system("leaks -q ircserv");
	// system("lsof -c ircserv");
	//Leaving Channel, Channel Removal, Passing Ownership.
}
