#include <iostream>                // для вводу/виводу
#include <string>                  // для string
#include <httplib.h>               // бібліотека для HTTP запитів (як curl, але в коді)
#include <nlohmann/json.hpp>       // бібліотека для роботи з JSON
#include <winsock2.h>              // сокети Windows
#include <ws2tcpip.h>              // IP та інші функції

#pragma comment(lib, "ws2_32.lib") // бібліотека сокетів

using namespace std;
using json = nlohmann::json;       // просто скорочення

string getHoroscope(const string& sign, const string& day) {
    httplib::Client cli("https://horoscope-app-api.vercel.app");    // створюємо клієнт до сайту
    string path = "/api/v1/get-horoscope/daily?sign=" + sign + "&day=" + day; // будуємо шлях до API
    auto res = cli.Get(path.c_str());      // робимо GET-запит
    if (res && res->status == 200) return res->body; // якщо успіх — повертаємо тіло відповіді
    return "Error retrieving data.";       // якщо щось не так — пишемо помилку
}

int main() {
    WSADATA wsaData;                      // структура для ініціалізації сокетів
    WSAStartup(MAKEWORD(2, 2), &wsaData); // запускаємо Winsock

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP-серверний сокет

    sockaddr_in serverAddr;               // структура з адресою сервера
    serverAddr.sin_family = AF_INET;      // тип адреси — IPv4
    serverAddr.sin_port = htons(8080);    // порт — 8080
    serverAddr.sin_addr.s_addr = INADDR_ANY; // приймаємо з будь-якої IP-адреси

    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); // прив'язуємо сокет до порту
    listen(serverSocket, SOMAXCONN);      // слухаємо нові підключення

    SOCKET clientSocket;                  // сокет для клієнта
    sockaddr_in clientAddr;              // адреса клієнта
    int clientSize = sizeof(clientAddr); // розмір адреси

    while ((clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize)) != INVALID_SOCKET) {
        char buffer[1024] = { 0 };        // буфер для прийому запиту від клієнта

        recv(clientSocket, buffer, sizeof(buffer), 0); // читаємо, що прислав клієнт

        istringstream iss(buffer);       // розділяємо строку на sign і day
        string sign, day;
        iss >> sign >> day;

        string result = getHoroscope(sign, day); // запит до API — отримуємо гороскоп

        send(clientSocket, result.c_str(), result.size(), 0); // шлемо гороскоп назад клієнту
        closesocket(clientSocket);        // закриваємо сокет клієнта після відповіді
    }

    closesocket(serverSocket);            // закриваємо серверний сокет після завершення
    WSACleanup();                         // прибираємо ресурси Winsock

    return 0;
}
