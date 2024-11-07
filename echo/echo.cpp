#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7
#define BUFFER_SIZE 1024

int main() {
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = {0};
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		return 1;
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		return 1;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		return 1;
	}
	
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		return 1;
	}
	
	std::cout << "Эхо-сервер запущен на порту: " << PORT << std::endl;
	
	while (true) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			perror("accept");
			return 1;
		}
		memset(buffer, 0, sizeof(buffer));
		read(new_socket, buffer, BUFFER_SIZE);
		send(new_socket, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;
		close(new_socket);
	}
	
	return 0;
}
