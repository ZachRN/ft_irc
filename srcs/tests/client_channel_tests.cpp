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
	std::cout << "Add Client, Set Nickname, User Nickname, basic Correct\n----------------" << std::endl;
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
	if (client->join_channel("Zachs_Home") != SUCCESS)
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
	std::cout << "Successfuly Created a channel, and gotten the channel from the client and server.\n----------------" << std::endl;
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
	std::cout << "Multiple Clients work, unique nicknames, non-unique users.\n----------------" << std::endl;
	return (SUCCESS);
}

int	multiple_channel_creation_test()
{
	std::cout << "Multiple User Creation Test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	client->join_channel("Zachs_Home");
	if (client->join_channel("Zachs_Home") == SUCCESS)
	{
		std::cout << "ERROR:Joined a Channel you already exist in" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Unable to join a channel you exist in." << std::endl;
	if (client->join_channel("Mikes_Home") != SUCCESS)
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
	std::cout << "Didn't join a channel 2x, Joined a second channel, was able to find second channel in client and server\n----------------" << std::endl;
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
	client->join_channel("Zachs_Home");
	ircServ.add_client(5);
	Client *clienttwo = ircServ.get_client(5);
	clienttwo->set_nickname("Mike");
	clienttwo->set_username("Mike");
	if (clienttwo->join_channel("Zachs_Home") != SUCCESS)
	{
		std::cout << "ERROR:Second Client unable to join first clients room" << std::endl;
		return (FAILURE);
	}
	else
		std::cout << "SUCCESS:Second Client can join first client channel" << std::endl;
	clienttwo->join_channel("Mikes_Home");
	if (client->join_channel("Mikes_Home") != SUCCESS)
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
	std::cout << "Both clients can create a room and join each others room.\n----------------" << std::endl;
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
	std::cout << "Able to remove when one client, multiple clients" << std::endl;
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
	//Leaving Channel, Channel Removal, Passing Ownership.
	// if (remove_user_from_channel() != SUCCESS)
		// return ;
}