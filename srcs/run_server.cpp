#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include "numeric_replies.hpp"
#include "server.hpp"

#include <ctime>

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

int check_client_sockets(Server server, int nfds, struct pollfd *fds)
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
				// THIS IS WHERE WE GO INTO COMMAND PROCESSING
				std::cout << "Received from client " << fds[i].fd << ": " << buffer << std::endl;

				// Send a response back to the client (for testing purposes)
				const char* response = "Received your message.\n";
				send(fds[i].fd, response, strlen(response), 0);
			} else if (bytesRead == 0)
			{
				std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
				server.remove_client(fds[i].fd);
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
	struct pollfd		fds[server.get_maxClients() + 1]; // +1 for the server socket
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
		int result = poll(fds, nfds, 5000);

		if (result == -1)
		{
			std::cerr << "Error in poll()." << std::endl;
			break;
		}

		// Check if the server socket has a new connection
		if (fds[0].revents & POLLIN)
		{
			int clientSocket = accept(server.get_socket(), (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocket != -1 && nfds < server.get_maxClients() + 1)
			{
				//fcntl(clientSocket, F_SETFL, O_NONBLOCK); // Set the client socket to non-blocking mode

				client_sockets.push_back(clientSocket);
				fds[nfds].fd = clientSocket;
				fds[nfds].events = POLLIN; // Check for incoming data on the client socket
				++nfds;
				server.add_client(clientSocket);
				std::cout << "New client connected #" << clientSocket << "." << std::endl;
				send(clientSocket, ":irc.localhost 001 test :Welcome to the Internet Relay Network test!test@localhost\n", 83, 0);
				send(clientSocket, ":irc.localhost 002 test :Your host is irc.localhost, running version 1.0\n", 73, 0);
				send(clientSocket, ":irc.localhost 003 test :This server was created sometime\n", 58, 0);
				send(clientSocket, ":irc.localhost 004 test irc.localhost 1.0 o o\n", 46, 0);
				send(clientSocket, ":irc.localhost 375 test :- irc.localhost Message of the Day - \n", 63, 0);
				send(clientSocket, ":irc.localhost 372 test :- Welcome to the Internet Relay Network test!test@localhost\n", 85, 0);
				send(clientSocket, ":irc.localhost 376 test :End of MOTD command\n", 45, 0);
			}
			else if (clientSocket != -1)
			{
				std::string message;
				message.append(":irc.localhost");
				message.append(" ");
				message.append(std::to_string(ERR_SERVERISFULL));
				message.append(" ");
				message.append("test :Server is full.\n");
				std::cerr << "Max clients reached." << std::endl;
				send(clientSocket, message.c_str(), message.length(), 0);
				close(clientSocket);
			}
			else
			{
				std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
			}
		}
		nfds = check_client_sockets(server, nfds, fds);
		for (int i = 0; i < nfds; ++i)
		{
			send(fds[i].fd, "PING :irc.localhost\n", 20, 0);
		}
	}
	return (SUCCESS);
}
