#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include "numeric_replies.hpp"
#include "server.hpp"
#include "input_process.hpp"

int send_msg(int sockfd, std::string msg)
{
	int bytesSent = send(sockfd, msg.c_str(), msg.length(), 0);
	if (bytesSent == -1)
	{
		std::cerr << "Error sending data to client " << sockfd << ": " << strerror(errno) << std::endl;
	}
	return (bytesSent);
}

void ping_all_clients(Server server)
{
	std::map<int, Client>::iterator it = server.get_clientList()->begin();
	while (it != server.get_clientList()->end())
	{
		if ((*it).second.get_lastPong() + 60 < std::time(nullptr))
		{
			std::cout << "Client " << (*it).second.get_nickname() << " timed out." << std::endl;
			close((*it).second.get_fd());
			it = server.get_clientList()->erase(it);
		}
		else if ((*it).second.get_verified() == true)
			send_msg((*it).second.get_fd(), "PING " + server.get_config().get_serverName() + "\r\n");
		++it;
	}
}

int start_listening(Server server)
{
	// Start listening for incoming connections
	if (listen(server.get_socket(), 5) == -1)
	{
		std::cerr << "Error listening on socket." << std::endl;
		close(server.get_socket());
		return (FAILURE);
	}

	std::cout << "Server listening on port " << server.get_port() << std::endl;
	return (SUCCESS);
}

int check_client_sockets(Server *server, int nfds, struct pollfd *fds)
{
	// Check for data on client sockets (starting from 1, as 0 is the server socket)
	for (int i = 1; i < nfds; ++i)
	{
		if (fds[i].revents & POLLIN)
		{
			char buffer[1024];
			ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
			if (bytesRead > 0)
			{
				buffer[bytesRead] = '\0';
				input_process(fds[i].fd, buffer, server);
				// std::cout << "Received from client " << fds[i].fd << ": " << buffer << std::endl;

				// Send a response back to the client (for testing purposes)
				// const char* response = "Received your message.\n";
				// send(fds[i].fd, response, strlen(response), 0);
			} else if (bytesRead == 0)
			{
				std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
				server->remove_client(fds[i].fd);
				close(fds[i].fd);
				// Shift the rest of the clients in the array to remove the disconnected client
				for (int j = i; j < nfds - 1; ++j)
				{
					fds[j] = fds[j + 1];
				}
				--nfds;
				--i; // To recheck the current index as the next client has been moved here
			} else
			{
				if (errno != EAGAIN && errno != EWOULDBLOCK)
				{
					std::cerr << "Error reading data from client " << fds[i].fd << ": " << strerror(errno) << std::endl;
					close(fds[i].fd);
					// Shift the rest of the clients in the array to remove the disconnected client
					for (int j = i; j < nfds - 1; ++j)
					{
						fds[j] = fds[j + 1];
					}
					--nfds;
					--i; // To recheck the current index as the next client has been moved here
				}
			}
		}
	}
	return (nfds);
}

int run_server(Server server)
{
	std::vector<int>	client_sockets;
	struct pollfd		fds[server.get_config().get_maxClients() + 1]; // +1 for the server socket
	int					nfds = 1; // Number of file descriptors being monitored (initially 1 for the server socket)
	struct sockaddr_in	clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);

	server.init_socket();
	if (start_listening(server) == FAILURE)
		return (FAILURE);

	fds[0].fd = server.get_socket();
	fds[0].events = POLLIN; // Check for incoming data on the server socket

	while (true)
	{
		// Use poll() to monitor sockets for I/O events
		int result = poll(fds, nfds, 1000);

		if (result == -1)
		{
			std::cerr << "Error in poll()." << std::endl;
			break;
		}

		// Check if the server socket has a new connection
		if (fds[0].revents & POLLIN)
		{
			int clientSocket = accept(server.get_socket(), (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocket != -1 && nfds < server.get_config().get_maxClients() + 1)
			{
				if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) // Set the client socket to non-blocking mode
				{
					std::cerr << "Could not set socket to non-blocking." << std::endl;
					return (FAILURE);
				}
				client_sockets.push_back(clientSocket);
				fds[nfds].fd = clientSocket;
				fds[nfds].events = POLLIN; // Check for incoming data on the client socket
				++nfds;
				server.add_client(clientSocket);
				std::cout << "New client connected #" << clientSocket << "." << std::endl;
			}
			else if (clientSocket != -1)
			{
				std::cerr << "Max clients reached." << std::endl;
				send_msg(clientSocket, ":localhost 999 test :Server is full.\n");
				close(clientSocket);
			}
			else
			{
				std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
			}
		}
		nfds = check_client_sockets(&server, nfds, fds);
		if (std::time(nullptr) % 5 == 0)
			ping_all_clients(server);
	}

	return (SUCCESS);
}
