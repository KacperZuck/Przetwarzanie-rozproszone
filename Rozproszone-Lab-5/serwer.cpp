#include <complex>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define ST_PORT 10000
#define MAX_K 2

HANDLE watki[MAX_K];
SOCKET si[MAX_K];
DWORD WINAPI funkcja_watku(void* argumenty);

struct WatekInfo {
    int index;
    SOCKET socket;
};

int main() {
    WSADATA wsas;
    WSAStartup(MAKEWORD(2, 2), &wsas);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ST_PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr*)&sa, sizeof(sa));
    listen(s, MAX_K);

    cout << "Nasluchuje port: " << ST_PORT << endl;

    sockaddr_in sc;
    int lenc = sizeof(sc);
    int polaczenia = 0;

    while (polaczenia < MAX_K) {
        SOCKET nowy = accept(s, (struct sockaddr*)&sc, &lenc);
        if (nowy == INVALID_SOCKET) {
            cerr << "Blad polaczenia: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "Nowe polaczenie: klient " << polaczenia << endl;
        si[polaczenia] = nowy;

        WatekInfo* info = new WatekInfo;
        info->index = polaczenia;
        info->socket = nowy;

        watki[polaczenia] = CreateThread(NULL, 0, funkcja_watku, info, 0, NULL);
        polaczenia++;
    }

    WaitForMultipleObjects(MAX_K, watki, TRUE, INFINITE);
    for (int i = 0; i < MAX_K; i++) {
        closesocket(si[i]);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}

DWORD WINAPI funkcja_watku(void* argumenty) {
    WatekInfo* info = (WatekInfo*)argumenty;
    int index = info->index;
    SOCKET local_s = info->socket;

    char buf[80];
    int bytes;

    while ((bytes = recv(local_s, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[bytes] = '\0';

        if (strcmp(buf, "KONIEC") == 0) {
            cout << "Klient " << index << " przerwanie " << endl;
            break;
        }

        cout << "Klient " << index << ": " << buf << endl;

        for (int i = 0; i < MAX_K; i++) {
            if (i != index && si[i] != INVALID_SOCKET) {
                send(si[i], buf, bytes, 0);
            }
        }
    }

    closesocket(local_s);
    return 0;
}
