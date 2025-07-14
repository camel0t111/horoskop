#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));

    string sign, day;
    cout << "Enter zodiac sign: ";
    cin >> sign;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> day;

    string message = sign + " " + day;
    send(sock, message.c_str(), message.size(), 0);

    char buffer[8192] = { 0 };
    recv(sock, buffer, sizeof(buffer), 0);
    cout << buffer << endl;

    closesocket(sock);
    WSACleanup();
    return 0;
}
