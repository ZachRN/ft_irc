#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

const int PORT = 6667; // IRC default port
const int MAX_CLIENTS = 10; // Maximum number of clients

int main() {
	int serverSocket;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	// Create a socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Error creating socket." << std::endl;
		return 1;
	}

	// Set the socket to non-blocking mode
	fcntl(serverSocket, F_SETFL, O_NONBLOCK);

	// Bind the socket to the specified port
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cerr << "Error binding socket to port " << PORT << std::endl;
		close(serverSocket);
		return 1;
	}

	// Start listening for incoming connections
	if (listen(serverSocket, 5) == -1) {
		std::cerr << "Error listening on socket." << std::endl;
		close(serverSocket);
		return 1;
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	std::vector<int> clients; // Vector to store connected client sockets

	while (true) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds); // Add the server socket to the set

		int maxFd = serverSocket; // The maximum file descriptor value

		// Add all connected client sockets to the set
		for (const auto& client : clients) {
			FD_SET(client, &readfds);
			maxFd = std::max(maxFd, client);
		}

		// Use select() to monitor sockets for I/O events
		if (select(maxFd + 1, &readfds, nullptr, nullptr, nullptr) == -1) {
			std::cerr << "Error in select()." << std::endl;
			break;
		}

		// Check if the server socket has a new connection
		if (FD_ISSET(serverSocket, &readfds)) {
			int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocket != -1) {
				// Set the client socket to non-blocking mode
				fcntl(clientSocket, F_SETFL, O_NONBLOCK);

				clients.push_back(clientSocket);
				std::cout << "New client connected." << std::endl;
			}
		}

		// Check for data on client sockets
		for (auto it = clients.begin(); it != clients.end(); ) {
			if (FD_ISSET(*it, &readfds)) {
				char buffer[1024];
				ssize_t bytesRead = recv(*it, buffer, sizeof(buffer) - 1, 0);
				if (bytesRead > 0) {
					buffer[bytesRead] = '\0';
					std::cout << "Received from client " << *it << ": " << buffer << std::endl;
					send(*it, "Acknowledged!\n", 14, 0);
				} else if (bytesRead == 0) {
					std::cout << "Client " << *it << " disconnected." << std::endl;
					close(*it);
					it = clients.erase(it);
					continue;
				} else {
					if (errno != EAGAIN && errno != EWOULDBLOCK) {
						std::cerr << "Error reading data from client " << *it << ": " << strerror(errno) << std::endl;
						close(*it);
						it = clients.erase(it);
						continue;
					}
				}
			}
			++it;
		}
	}

	// Close all client sockets
	for (const auto& client : clients) {
		close(client);
	}

	close(serverSocket); // Close the server socket

	return 0;
}
