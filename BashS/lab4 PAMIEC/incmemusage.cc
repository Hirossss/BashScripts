#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int main() {
    double*** array_of_arrays = NULL;
    int i, j, k;

    // Tworzenie tablicy wskaźników do tablic 2-wymiarowych
    array_of_arrays = (double***)malloc(SIZE * sizeof(double**));

    if (array_of_arrays == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci dla tablicy wskaźników\n");
        return 1;
    }

    // Inicjalizacja tablicy wskaźników
    for (i = 0; i < SIZE; i++) {
        array_of_arrays[i] = (double**)malloc(SIZE * sizeof(double*));
        if (array_of_arrays[i] == NULL) {
            fprintf(stderr, "Błąd alokacji pamięci dla tablicy %d\n", i);
            return 1;
        }
    }

    // Inicjalizacja tablic 2-wymiarowych
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            array_of_arrays[i][j] = (double*)malloc(SIZE * sizeof(double));
            if (array_of_arrays[i][j] == NULL) {
                fprintf(stderr, "Błąd alokacji pamięci dla tablicy %d, %d\n", i, j);
                return 1;
            }

            for (k = 0; k < SIZE; k++) {
                array_of_arrays[i][j][k] = 0.0;
            }
        }
    }

    printf("Tablica tablic została utworzona. Naciśnij dowolny klawisz, aby zakończyć...\n");
    getchar();

    // Zwalnianie zaalokowanej pamięci
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            free(array_of_arrays[i][j]);
        }
        free(array_of_arrays[i]);
    }
    free(array_of_arrays);

    return 0;
}
