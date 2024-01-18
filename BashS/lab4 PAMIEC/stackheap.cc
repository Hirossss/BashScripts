#include <stdio.h>
#include <stdlib.h>

void statyczna() {
    double tablica[1000000];  // Zmienna lokalna, automatyczna
    printf("Funkcja statyczna - Wcisnij Enter, aby kontynuowac...");
    getchar();  // Blokowanie programu
}

void dynamiczna() {
    double *tablica = malloc(1000000 * sizeof(double));  // Zmienna lokalna, dynamiczna
    printf("Funkcja dynamiczna - Wcisnij Enter, aby kontynuowac...");
    getchar();  // Blokowanie programu
    free(tablica);  // Zwolnienie zaalokowanej pamięci
}

int main() {
    statyczna();
    // Wstrzymanie programu
    printf("Wstrzymanie programu po funkcji statycznej - Wcisnij Enter...");
    getchar();

    dynamiczna();
    // Wstrzymanie programu
    printf("Wstrzymanie programu po funkcji dynamicznej - Wcisnij Enter...");
    getchar();

    return 0;
}

