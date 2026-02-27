// Created by zucek on 26.03.2025.
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")  // Linkowanie biblioteki dla Windows

#define PORT 10000

using namespace std;

int main() {
    WSADATA wsas;
    int result;
    WORD wersja = MAKEWORD(2, 2);

    result = WSAStartup(wersja, &wsas);
    if (result != 0) {
        cerr << "WSAStartup F: " << result << endl;
        return 1;
    }

    // Tworzenie gniazda
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Tworzenie socetu F: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = INADDR_ANY;  // Nasłuchiwanie na wszystkich interfejsach

    result = bind(serverSocket, (struct sockaddr*)&sa, sizeof(sa));

    if (result == SOCKET_ERROR) {
        cerr << "Bind F: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    result = listen(serverSocket, 2);
    if (result == SOCKET_ERROR) {
        cerr << "Listen F: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Serwer nasluchuje na porcie " << PORT << "..." << endl;

    struct sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept F: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "Nowe polaczenie" << endl;

        char buf[80];
        int bytesReceived;
        while ((bytesReceived = recv(clientSocket, buf, sizeof(buf) - 1, 0)) > 0) {
            buf[bytesReceived] = '\0';  // Null-terminate the string
            cout << "Odebrano: " << buf << endl;

            if (strcmp(buf, "KONIEC") == 0) {
                cout << "Zamykanie połączenia..." << endl;
                closesocket(clientSocket);
                break;
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
