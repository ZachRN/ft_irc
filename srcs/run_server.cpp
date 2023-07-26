#include <fcntl.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include "server.hpp"

int start_listening(int socket)
{
	// Start listening for incoming connections
	if (listen(socket, 5) == -1) {
		std::cerr << "Error listening on socket." << std::endl;
		close(socket);
		return (FAILURE);
	}

	std::cout << "Server listening on port " << socket << std::endl;
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

				// Send a response back to the client
				const char* response = "Received your message.\n";
				send(fds[i].fd, response, strlen(response), 0);
			} else if (bytesRead == 0)
			{
				std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
				// THIS IS WHERE WE CLEAN UP THE CLIENT
				close(fds[i].fd);
				// Shift the rest of the clients in the array to remove the disconnected client
				for (int j = i; j < nfds - 1; ++j) {
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
					for (int j = i; j < nfds - 1; ++j) {
						fds[j] = fds[j + 1];
					}
					--nfds;
					--i; // To recheck the current index as the next client has been moved here
				}
			}
		}
	}
	return (SUCCESS);
}

int run_server(Server server)
{
	std::vector<int>	client_sockets;
	struct pollfd		fds[server.get_maxClients() + 1]; // +1 for the server socket
	int					nfds = 1; // Number of file descriptors being monitored (initially 1 for the server socket)
	struct sockaddr_in	clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);

	server.init_socket();
	if (start_listening(server.get_socket()) == FAILURE)
		return (FAILURE);

	fds[0].fd = server.get_socket();
	fds[0].events = POLLIN; // Check for incoming data on the server socket

	while (true) {
		// Use poll() to monitor sockets for I/O events
		int result = poll(fds, nfds, -1); // -1 for indefinite blocking

		if (result == -1) {
			std::cerr << "Error in poll()." << std::endl;
			break;
		}

		// Check if the server socket has a new connection
		if (fds[0].revents & POLLIN) {
			int clientSocket = accept(server.get_socket(), (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocket != -1) {
				fcntl(clientSocket, F_SETFL, O_NONBLOCK); // Set the client socket to non-blocking mode

				client_sockets.push_back(clientSocket);
				fds[nfds].fd = clientSocket;
				fds[nfds].events = POLLIN; // Check for incoming data on the client socket
				++nfds;

				std::cout << "New client connected." << std::endl;
			}
		}
		check_client_sockets(server, nfds, fds);
	}
	return (SUCCESS);
}