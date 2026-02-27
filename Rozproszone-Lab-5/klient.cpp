#include <complex>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define n_watki 2
HANDLE watki[n_watki];
DWORD WINAPI funkcja_watku(void* argumenty); // Wątek odbierający wiadomości

SOCKET s;

int main(int argc, char* argv[]) {

    struct sockaddr_in sa;
    WSADATA wsas;
    WORD wersja = MAKEWORD(2, 0);
    WSAStartup(wersja, &wsas);

    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(10000);
    sa.sin_addr.s_addr = inet_addr(argv[1]);

    int result = connect(s, (struct sockaddr FAR*)&sa, sizeof(sa));
    if (result == SOCKET_ERROR) {
        printf("\n Blad polaczenia !\n");
        return 1;
    } else {
        cout << "Polaczenie udane" << endl;
    }

    watki[0] = CreateThread(NULL, 0, funkcja_watku, NULL, 0, NULL);

    char buf[80];
    while (true) {
        fgets(buf, 80, stdin);
        int dlug = strlen(buf);
        if (dlug > 0 && buf[dlug - 1] == '\n') buf[dlug - 1] = '\0';

        send(s, buf, strlen(buf), 0);

        if (strcmp(buf, "KONIEC") == 0) break;
    }

    closesocket(s);
    WSACleanup();

    return 0;
}

DWORD WINAPI funkcja_watku(void* argumenty) {
    cout << "Przejscie do odbierania" << endl;
    char buf[80];
    int bytes;
    while ((bytes = recv(s, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[bytes] = '\0';
        cout << "\n[Serwer]: " << buf << endl;
        fflush(stdout);
    }
    cout << "Koniec odbierania" << endl;
    return 0;
}
