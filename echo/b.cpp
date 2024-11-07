#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 7
#define SERVER_IP "127.0.0.1"

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(s, (const struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        cerr << "Ошибка установки соединения" << endl;
        close(s);
        return 1;
    }

    char msg[] = "hellon";
    cout << "Отправлено" << endl;
    send(s, msg, sizeof(msg), 0);

    char buf[256];
    memset(buf, 0, sizeof(buf)); 
    recv(s, buf, sizeof(buf), 0);
    
    cout << "Ответ от сервера: " << buf << endl;
    
    close(s);
    return 0;
}
