#include "genetic_algorithm.h"
#include <stdlib.h>
#include <time.h>

// Aplica la selección por ruleta para obtener un índice de individuo en [0, MIU-1] 
int roulette_selection(const Individual population[MIU], double sum_fit) {
    double r = ((double)rand() / RAND_MAX) * sum_fit;
    double acumulado = 0.0;
    for (int i = 0; i < MIU; i++) {
        acumulado += population[i].fitness;
        if (acumulado >= r)
            return i;
    }
    return MIU - 1; // Por si hay error numérico
}

// Cruce de un punto entre dos cromosomas parent1 y parent2, generando dos hijos child1 y child2 
void crossover_one_point(const Individual *parent1, const Individual *parent2, Individual *child1, Individual *child2) {
    int point = rand() % (NB - 1) + 1;

    // Copiamos la parte inicial
    for (int i = 0; i < point; i++) {
        child1->chromosome[i] = parent1->chromosome[i];
        child2->chromosome[i] = parent2->chromosome[i];
    }
    // Copiamos la parte final
    for (int i = point; i < NB; i++) {
        child1->chromosome[i] = parent2->chromosome[i];
        child2->chromosome[i] = parent1->chromosome[i];
    }
}

/* Mutación bit a bit con probabilidad pmu */
void mutate_chromosome(Individual *ind, double pmu) {
    for (int i = 0; i < NB; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < pmu)
            ind->chromosome[i] = !ind->chromosome[i];  // Invierte el bit
    }
}