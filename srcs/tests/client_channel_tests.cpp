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
		std::cout << "User Could not be created." << std::endl;
		return (FAILURE);
	}
	Client *client = ircServ.get_client(4);
	if (client->set_nickname("Zach") != SUCCESS)
	{
		std::cout << "User Could not set nickname." << std::endl;
		return (FAILURE);
	}
	if (client->set_username("Zach") != SUCCESS)
	{
		std::cout << "User Could not set username." << std::endl;
		return (FAILURE);
	}
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
		std::cout << "Channel Failed to Create." << std::endl;
		return (FAILURE);
	}
	if (client->get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "Failed to add channel to client channelList" << std::endl;
		return (FAILURE);
	}
	if (ircServ.get_channel("Zachs_Home") == nullptr)
	{
		std::cout << "Failed to add channel to server channelList" << std::endl;
		return (FAILURE);
	}
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
		std::cout << "Error, added multiple clients with same FD" << std::endl;
		return (FAILURE);
	}
	if (ircServ.add_client(5) != SUCCESS)
	{
		std::cout << "Error, adding multiple clients with different FD's didn't work." << std::endl;
		return (FAILURE);
	}
	Client *clienttwo = ircServ.get_client(5);
	if (clienttwo->set_nickname("Zach") == SUCCESS)
	{
		std::cout << "Error, multiple clients ith same nickname" << std::endl;
		return (FAILURE);
	}
	clienttwo->set_nickname("Mike");
	if (clienttwo->set_username("Zach") != SUCCESS)
	{
		std::cout << "Error, user names should be non-unique" << std::endl;
		return (FAILURE);
	}
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
		std::cout << "Joined a Channel you already exist in" << std::endl;
		return (FAILURE);
	}
	if (client->join_channel("Mikes_Home") != SUCCESS)
	{
		std::cout << "Unable to join multiple channels" << std::endl;
		return (FAILURE);
	}
	if (client->get_channel("Mikes_Home") == nullptr)
	{
		std::cout << "Failed to add channel to client channelList" << std::endl;
		return (FAILURE);
	}
	if (ircServ.get_channel("Mikes_Home") == nullptr)
	{
		std::cout << "Failed to add channel to server channelList" << std::endl;
		return (FAILURE);
	}
	std::cout << "Didn't join a channel 2x, Joined a second channel, was able to find second channel in client and server" << std::endl;
	return (SUCCESS);
}

int	multiple_users_one_channel_test()
{
	std::cout << "Multiple User Creation Test" << std::endl;
	Server ircServ("pass", 123);
	ircServ.add_client(4);
	Client *client = ircServ.get_client(4);
	client->set_nickname("Zach");
	client->set_username("Zach");
	client->join_channel("Zachs_Home");
	Client *clienttwo = ircServ.get_client(5);
	clienttwo->set_nickname("Mike");
	clienttwo->set_username("Mike");
	if (clienttwo->join_channel("Zachs_Home") != SUCCESS)
	{
		std::cout << "Second Client unable to join first clients room" << std::endl;
		return (FAILURE);
	}
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
	if (multiple_users_one_channel_test() != SUCCESS)
		return ;
}