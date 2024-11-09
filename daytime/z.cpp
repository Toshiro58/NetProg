#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

using namespace std;

#define PORT 13
#define SERVER_IP "127.16.40.1"
#define BUFFER_SIZE 256

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        cerr << "Ошибка создания сокета" << endl;
        return 1;
    }

    struct sockaddr_in self_addr;
    memset(&self_addr, 0, sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(0);
    self_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (const struct sockaddr*)&self_addr, sizeof(self_addr)) == -1) {
        cerr << "Ошибка привязки сокета" << endl;
        close(s);
        return 1;
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    
    string message;
    cout << "Введите сообщение: ";
    getline(cin, message);

    
    if (sendto(s, message.c_str(), message.length(), 0, 
               (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        cerr << "Ошибка отправки данных" << endl;
        close(s);
        return 1;
    }
    cout << "Сообщение отправлено" << endl;

    
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    socklen_t addr_len = sizeof(srv_addr);
    
    if (recvfrom(s, buf, BUFFER_SIZE - 1, 0, 
                 (struct sockaddr*)&srv_addr, &addr_len) == -1) {
        cerr << "Ошибка получения данных" << endl;
        close(s);
        return 1;
    }
    
    cout << "Ответ от сервера: " << buf << endl;
    
    close(s);
    return 0;
}
