#include <iostream>              // для вводу/виводу
#include <string>                // для string (введення знака і дати)
#include <winsock2.h>            // основне для сокетів в Windows
#include <ws2tcpip.h>            // inet_pton — IP у формат сокета

#pragma comment(lib, "ws2_32.lib")  // підключаємо бібліотеку сокетів

using namespace std;

int main() {
    WSADATA wsaData;                             // спец. структура для запуску сокетів
    WSAStartup(MAKEWORD(2, 2), &wsaData);        // стартуємо Winsock (обов'язково для Windows)

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // створюємо TCP сокет (клієнтський)
    sockaddr_in serv_addr;                       // структура з адресою сервера

    serv_addr.sin_family = AF_INET;              // тип адреси — IPv4
    serv_addr.sin_port = htons(8080);            // порт — 8080, в мережевому порядку байтів
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);  // переводимо IP з строки в формат, що розуміє система

    connect(sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)); // намагаємось підключитись до сервера

    string sign, day;                            // sign — знак зодіаку, day — дата

    cout << "Enter zodiac sign: ";               // просимо ввести знак
    cin >> sign;

    cout << "Enter date (YYYY-MM-DD): ";         // просимо ввести дату
    cin >> day;

    string message = sign + " " + day;           // формуємо повідомлення, яке шлемо на сервер

    send(sock, message.c_str(), message.size(), 0); // шлемо це все на сервер

    char buffer[8192] = { 0 };                   // буфер для отримання відповіді (JSON)

    recv(sock, buffer, sizeof(buffer), 0);       // чекаємо на відповідь від сервера

    cout << buffer << endl;                      // виводимо відповідь у консоль

    closesocket(sock);                           // закриваємо сокет після завершення
    WSACleanup();                                // прибираємо всі ресурси, що виділив Winsock

    return 0;
}
