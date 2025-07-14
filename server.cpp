#include <iostream>
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
using json = nlohmann::json;

string getHoroscope(const string& sign, const string& day) {
    httplib::Client cli("https://horoscope-app-api.vercel.app");
    string path = "/api/v1/get-horoscope/daily?sign=" + sign + "&day=" + day;
    auto res = cli.Get(path.c_str());
    if (res && res->status == 200) return res->body;
    return "Error retrieving data.";
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    while ((clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize)) != INVALID_SOCKET) {
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);

        istringstream iss(buffer);
        string sign, day;
        iss >> sign >> day;

        string result = getHoroscope(sign, day);
        send(clientSocket, result.c_str(), result.size(), 0);
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
