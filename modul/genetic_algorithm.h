#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "individual.h"

#define MIU 100          // Tamaño de la población (número de padres)
#define PCR 0.9          // Probabilidad de cruce
#define PMU 1.0 / NB     // Probabilidad de mutación por bit

// Declaraciones de funciones
int roulette_selection(const Individual population[MIU], double sum_fit);
void crossover_one_point(const Individual *parent1, const Individual *parent2, Individual *child1, Individual *child2);
void mutate_chromosome(Individual *ind, double pmu);

#endif // GENETIC_ALGORITHM_H