#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

int is_fibonnaci(int n){
    if(n<3){
        return 1;
    }
    else{
        return is_fibonnaci(n-1) + is_fibonnaci(n-2);
    }
}

bool is_prime(int x){
    if( x == 2){ return true;}
    if(x%2 ==0 || x ==1){
            return false;
    }
    for(int i=3; i<x; i+=2){
        if(x%i == 0){
            return false;
        }
    }
    return true ;        
}

int main (){
    int n, ktora=0;

   


    cout << "Liczba pierwsza w ciagu Fib: "<< endl;
    clock_t start = clock();
    for(int i=1; i<1000000000; i++){
        
        n = is_fibonnaci(i);
        if(n >= 1000000000) break;
        if(is_prime(n)) {
            ktora++;
            cout << ktora <<" - "<< n << endl;
        }
    
    }
    clock_t end = clock();
    cout << (float)(end - start)/CLOCKS_PER_SEC << endl ;


    // 2
    int pid ,start_i=0, end_i=500000000;;
    n, ktora=0;

    pid = fork(); 
    if ( pid == 0){
        printf (" Zglasza sie proces potomny % d \n " , getpid () ) ;
        start_i=0, end_i=500000000;
    }else{
        printf (" Zlasza sie proces macierzysty %d . Potomek %d \n " ,getpid () , pid ) ;
        start_i=500000000, end_i=1000000000;
    }
    
      start = clock();
    for(int i=1; i<1000000000; i++){
        
        n = is_fibonnaci(i);
        if(n >= 1000000000) break;
        if(is_prime(n)) {
            ktora++;
            cout << ktora <<" - "<< n << endl;
        }
    
    }
     end = clock();
    cout << (float)(end - start)/CLOCKS_PER_SEC << endl ;


    return 0;
}