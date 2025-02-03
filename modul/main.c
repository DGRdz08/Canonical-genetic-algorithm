#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "individual.h"
#include "genetic_algorithm.h"

#define T 10             // Número de generaciones (criterio de paro)

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));

    Individual population[MIU]; // Población inicial

    // Inicialización de la población
    for (int i = 0; i < MIU; i++) {
        random_chromosome(&population[i]);
        decode(&population[i]);
        population[i].fitness = fitness_function(population[i].x);
    }

    for (int gen = 1; gen <= T; gen++) {
        double sum_fitness = 0.0;
        for (int i = 0; i < MIU; i++)
            sum_fitness += population[i].fitness;

        Individual new_population[MIU]; // Nueva población

        for (int i = 0; i < MIU; i += 2) {
            // Seleccionar dos padres por ruleta
            int p1 = roulette_selection(population, sum_fitness);
            int p2;
            do {
                p2 = roulette_selection(population, sum_fitness);
            } while (p1 == p2);

            // Copiamos por defecto los padres como hijos (por si no hay cruce)
            new_population[i] = population[p1];
            new_population[i+1] = population[p2];

            // Decidir si se realiza cruce
            double rc = (double)rand() / RAND_MAX;
            if (rc < PCR) 
                crossover_one_point(&population[p1], &population[p2], &new_population[i], &new_population[i+1]);

            // Se muta cada hijo
            mutate_chromosome(&new_population[i], PMU);
            mutate_chromosome(&new_population[i+1], PMU);

            // Decodificar y calcular el fitness de los nuevos individuos
            decode(&new_population[i]);
            decode(&new_population[i+1]);
            new_population[i].fitness = fitness_function(new_population[i].x);
            new_population[i+1].fitness = fitness_function(new_population[i+1].x);
        }

        // Actualizar la población
        for (int i = 0; i < MIU; i++)
            population[i] = new_population[i];

        // Encontrar el mejor individuo de la generación
        int best_idx = 0;
        for (int i = 1; i < MIU; i++) 
            if (population[i].fitness > population[best_idx].fitness)
                best_idx = i;

        printf("Generacion %d \t -> Mejor x = %.2f, fitness = %.2f\n", gen, population[best_idx].x, population[best_idx].fitness);
    }

    // Encontrar el mejor individuo final
    int best_idx = 0;
    for (int i = 1; i < MIU; i++) 
        if (population[i].fitness > population[best_idx].fitness)
            best_idx = i;

    printf("\nMejor individuo tras %d generaciones:\n", T);
    printf("  Cromosoma: ");
    for (int b = 0; b < NB; b++) 
        printf("%d", population[best_idx].chromosome[b]);
    
    printf("\n  x = %.2f\n", population[best_idx].x);
    printf("  Fitness = %.2f\n", population[best_idx].fitness);

    return 0;
}