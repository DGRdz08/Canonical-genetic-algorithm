#include "individual.h"
#include <stdlib.h>
#include <time.h>

/* Genera un cromosoma binario aleatorio */
void random_chromosome(Individual *ind) {
    for (int i = 0; i < NB; i++)
        ind->chromosome[i] = rand() % 2;
}

/* Decodifica un cromosoma binario a un valor real */
void decode(Individual *ind) {
    double value = 0.0;
    double power_of_2 = 1.0; // 2^0

    for (int i = NB - 1; i >= 0; i--) { // Recorre el cromosoma de derecha a izquierda
        if (ind->chromosome[i] == 1)
            value += power_of_2;
        power_of_2 *= 2.0;
    }
    ind->x = value * SCALE;
}

// Función objetivo a maximizar: f(x) = 30 - (x-5)^2 
double fitness_function(double x) {
    return 30.0 - (x - 5.0) * (x - 5.0);
}