#include <iostream>
#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0 , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(13);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    cout << "Сервер запущен" << endl;
    time_t current_time;
    while(true) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char buffer[256];
        ssize_t recv_bytes = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_bytes == -1) {
            cerr << "Ошибка при получении данных" << endl;
            continue;
        }
        time(&current_time);
        string datetime = ctime(&current_time);
        cout << buffer << endl;
        ssize_t sent_bytes = sendto(s, datetime.c_str(), datetime.length(), 0, (struct sockaddr *)&client_addr, addr_len);
        if (sent_bytes == -1) {
            cerr << "Ошибка при отправке данных" << endl;
        }
    }
    close(s);
    return 0;
}
