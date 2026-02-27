#include <iostream>
#include <math.h>
#include <valarray>
using namespace std;

#define MAX_PRIME 10000
#define MAX_I 1000000000
#define MAX_THREAD 16

int primes[MAX_PRIME]; // tablica z liczbami pierwszymi
int curr_p = 0; // ile w tablicy jest wpisanych liczb pierwszych
pthread_mutex_t m_p; // semafor chroniacy dostepu do tablicy
int pthread_mutex_unlock ( pthread_mutex_t * m ); // otwarcie
int pthread_mutex_lock ( pthread_mutex_t * m ); // zamkniecie


int find_prime(int start_i ) {

    for (int i = start_i; i <= MAX_I; i++) {
        if (i < 3) return 2;
        bool is_prime = true;
        if (i%2 == 0){ is_prime = false;}
        else {
            for (int j=3; j<i; j+=2) {
                if (i%j == 0) {
                    is_prime = false;
                    break;
                }
            }
        }
        if (is_prime) {
            return i;
        }
    }
}

void * f ( void * i ){
    int n = 0;
    while(n < MAX_I) {
        int prime = find_prime(n);
        //TODO -- KRYTYCZNA
        pthread_mutex_lock (&m_p);
        if (curr_p == MAX_PRIME) {
            cout << "Prime max number\n";
            pthread_mutex_unlock (&m_p);
            break;
        }
        primes[curr_p] = prime;
        curr_p++;
        pthread_mutex_unlock (&m_p);
        n = prime+1;

    }
        printf(" Thread says : end work \n ") ;
return NULL ;
}

int main() {

    pthread_t w[MAX_THREAD] ;
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_create (& w[i], NULL , f , NULL ) ;
    }
    for (int i = 0; i < MAX_THREAD; i++) {
        pthread_join (w[i] , NULL ) ;
    }

    printf ("Znalezione liczby pierwsze: \n ") ;
    for (int i = 0; i < MAX_PRIME; i++) {
        cout << primes[i] << " " ;
    }
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.