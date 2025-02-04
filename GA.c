#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define T 10             // Número de generaciones (criterio de paro)
#define MIU 100          // Tamaño de la población (número de padres)
#define NB 10            // Número de bits por individuo
#define PCR 0.9          // Probabilidad de cruce
#define PMU 1.0 / NB     // Probabilidad de mutación por bit
#define SCALE 0.01       // Factor para pasar de binario a x real:   x = (entero) * SCALE

// Estructura para representar a un individuo con cromosoma compacto
typedef struct {
    unsigned int chromosome;  // Cromosoma representado como un entero (NB bits)
    double fitness;           // Valor de fitness
    double x;                 // Valor real decodificado
} Individual;

// Función para calcular el fitness
double fitness_function(double x) {
    return 30.0 - (x - 5.0) * (x - 5.0);
}

// Genera un cromosoma aleatorio con NB bits
void random_chromosome(Individual *ind) {
    ind->chromosome = rand() % (1 << NB); // Generar un número aleatorio con NB bits
}

// Decodifica el cromosoma a un valor real
void decode(Individual *ind) {
    ind->x = (double)(ind->chromosome) * SCALE;
}

// Selección por ruleta
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

// Cruce de un punto
void crossover_one_point(const Individual *parent1, const Individual *parent2, Individual *child1, Individual *child2) {
    int point = rand() % NB; // Punto de cruce
    unsigned int mask = (1 << point) - 1; // Máscara para los bits menos significativos

    child1->chromosome = (parent1->chromosome & mask) | (parent2->chromosome & ~mask);
    child2->chromosome = (parent2->chromosome & mask) | (parent1->chromosome & ~mask);
}

// Mutación bit a bit
void mutate_chromosome(Individual *ind, double pmu) {
    for (int i = 0; i < NB; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < pmu) {
            ind->chromosome ^= (1 << i); // Cambiar el i-ésimo bit
        }
    }
}

int main() {
    srand((unsigned)time(NULL));

    Individual population[MIU]; // Población inicial

    // Inicializar la población
    for (int i = 0; i < MIU; i++) {
        random_chromosome(&population[i]);
        decode(&population[i]);
        population[i].fitness = fitness_function(population[i].x);
    }

    // Proceso evolutivo
    for (int gen = 1; gen <= T; gen++) {
        double sum_fitness = 0.0;
        for (int i = 0; i < MIU; i++)
            sum_fitness += population[i].fitness;

        Individual new_population[MIU]; // Nueva población

        for (int i = 0; i < MIU; i += 2) {
            int p1 = roulette_selection(population, sum_fitness);
            int p2;
            do {
                p2 = roulette_selection(population, sum_fitness);
            } while (p1 == p2);

            new_population[i] = population[p1];
            new_population[i+1] = population[p2];

            double rc = (double)rand() / RAND_MAX;
            if (rc < PCR)
                crossover_one_point(&population[p1], &population[p2], &new_population[i], &new_population[i+1]);

            mutate_chromosome(&new_population[i], PMU);
            mutate_chromosome(&new_population[i+1], PMU);

            decode(&new_population[i]);
            decode(&new_population[i+1]);
            new_population[i].fitness = fitness_function(new_population[i].x);
            new_population[i+1].fitness = fitness_function(new_population[i+1].x);
        }

        for (int i = 0; i < MIU; i++)
            population[i] = new_population[i];

        int best_idx = 0;
        for (int i = 1; i < MIU; i++)
            if (population[i].fitness > population[best_idx].fitness)
                best_idx = i;

        printf("Generacion %d -> Mejor x = %.2f, fitness = %.2f\n", gen, population[best_idx].x, population[best_idx].fitness);
    }

    return 0;
}
