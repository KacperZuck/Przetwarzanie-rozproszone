#include <iostream>
#include <ctime>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
using namespace std;

#define NUM 1000000000
#define ilosc_w 2
int is_fibonnaci(int n){
    if (n < 3) return 1;
    int a = 1, b = 1, c=0;
    for (int i = 3; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}


bool is_prime(int x){
    if( x == 2){ return true;}
    if(x%2 ==0 || x ==1){
            return false;
    }
    for(int i=3; i<x; i+=2){  // TODO - dla sprawdzania sqrt od x jest 100x szybsza
        if(x%i == 0){
            return false;
        }
    }
    return true ;
}

int oblicz( int start_i, int end_i){
    int n =0, licznik=0;
    for(int i=start_i; i<end_i; i++){
        n = is_fibonnaci(i);
        if(n >= NUM || n < 0 ) break;
        if(is_prime(n)) {
            licznik++;
            cout<< " " << n << endl;
        }
    }
    return licznik;
}



#define n_watki 1
HANDLE watki[n_watki];
DWORD WINAPI funkcja_watku(void* argumenty);
void SetExitCodeThread(DWORD dword);

DWORD SuspendThread(HANDLE hThread);
DWORD ResumeThread (
HANDLE hThread
) ;

int main (){
    int n;
    int nnn=0;
    cout << "Liczba pierwsza w ciagu Fib (zwykła metoda): "<< endl;
    clock_t start = clock();
      for(int i=1; i<NUM; i++){

          n = is_fibonnaci(i);
          if(n >= NUM) break;
         if(is_prime(n)) {
              nnn++;
              cout << nnn <<" - "<< n << endl;
          }

     }
     clock_t end = clock();
    cout << "Czas bez optymalizacji: " << (float)(end - start)/CLOCKS_PER_SEC << endl ;


    // 2
    cout << endl << "Liczba pierwsza w ciagu Fib (wielowatkowa metoda, ilosc: "<< n_watki << "): "<< endl;

    DWORD id;
    int dane[n_watki][2];// = { {1,NUM/2}, {NUM/2, NUM} };    // watki x 2
    dane[0][0] = 1;
    dane[0][1] = NUM/(n_watki);
    for (int i =1; i<n_watki; i++) {
        dane[i][0] = i*(NUM/n_watki);
        dane[i][1] = (i+1)*(NUM/n_watki);
    }

    clock_t st2 = clock();
    for (int i = 0; i < n_watki; i++)
    {
        watki[i] = CreateThread(
            NULL, // atrybuty bezpieczenstwa
            0, // inicjalna wielkosc stosu
            funkcja_watku, // funkcja watku
            (void*)&dane[i],// dane dla funkcji watku
            0, // flagi utworzenia
            &id);
        SuspendThread (watki[i]);
        // if (watki[i] != INVALID_HANDLE_VALUE)
        // {
        //     // cout << "Jest watek, zakres: " << dane[i][0] << ", " << dane[i][1] << ", id: " << id << endl;
        // }
    }

    WaitForMultipleObjects(n_watki, watki, TRUE, INFINITE);
    DWORD tmp;
    int wynik=0;
    clock_t edn2 = clock();

    for (int i = 0; i < n_watki; i++) {
        tmp = 0;
       // WaitForSingleObject(watki[i],INFINITE);
      //  GetExitCodeThread(watki[i], &tmp);
        wynik += tmp;
    }
    cout << wynik << endl;
    cout << "Czas caSDSDlkowity: " << (float)(edn2 - st2) / CLOCKS_PER_SEC << " sekundy\n";

    return 0;
}

DWORD WINAPI funkcja_watku(void* argumenty)
{
    unsigned int licznik = 0;
    int* moje_dane = (int*)argumenty;

    int n = oblicz(moje_dane[0], moje_dane[1]);
    //cout << moje_dane[0] << " " << moje_dane[1] << endl;
   // ExitThread(n);
    return 0;
}
