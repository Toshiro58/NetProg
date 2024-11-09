#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 13
#define SERVER_IP "127.16.40.1"
#define BUFFER_SIZE 256

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    struct sockaddr_in self_addr;
    memset(&self_addr, 0, sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(0);
    self_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (const struct sockaddr*)&self_addr, sizeof(self_addr)) == -1) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(s);
        return 1;
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(s, (const struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        std::cerr << "Ошибка установки соединения" << std::endl;
        close(s);
        return 1;
     }

    std::string message;
    std::cout << "Введите сообщение: ";
    std::getline(std::cin, message);

    char msg[message.size() + 1];
    strcpy(msg, message.c_str());

    if (sendto(s, msg, strlen(msg), 0, (const struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        std::cerr << "Ошибка отправки данных" << std::endl;
        close(s);
        return 1;
    }

    char buf[BUFFER_SIZE];
    socklen_t len = sizeof(srv_addr);
    if (recvfrom(s, buf, BUFFER_SIZE, 0, (struct sockaddr*)&srv_addr, &len) == -1) {
        std::cerr << "Ошибка получения данных" << std::endl;
        close(s);
        return 1;
    }

    buf[BUFFER_SIZE - 1] = '\0'; 
    std::cout << "Ответ от сервера: " << buf << std::endl;

    close(s);
    return 0;
}
