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

// Definimos una estructura para representar a un individuo
typedef struct {
    int chromosome[NB];  // Cromosoma binario
    double fitness;      // Valor de fitness
    double x;            // Valor real decodificado
} Individual;

// Función objetivo a maximizar: f(x) = 30 - (x-5)^2 
double fitness_function(double x) {
    return 30.0 - (x - 5.0) * (x - 5.0);
}

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
    // Selecciona aleatoriamente un punto de cruce
    // El rango va de 1 a NB-1 para no cruzar en los extremos
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

        // Encontrar el mejor individuo de la generación actual (elitismo)
        int best_idx = 0;
        for (int i = 1; i < MIU; i++) 
            if (population[i].fitness > population[best_idx].fitness)
                best_idx = i;

        // Copiar el mejor individuo a la nueva población
        new_population[0] = population[best_idx];

        for (int i = 1; i < MIU; i += 2) {
            // Seleccionar dos padres por ruleta
            int p1 = roulette_selection(population, sum_fitness);
            int p2;
            do {
                p2 = roulette_selection(population, sum_fitness);
            } while (p1 == p2);

            // Copiamos por defecto los padres como hijos (por si no hay cruce)
            new_population[i] = population[p1];
            if (i + 1 < MIU) 
                new_population[i + 1] = population[p2];

            // Decidir si se realiza cruce
            double rc = (double)rand() / RAND_MAX;
            if (rc < PCR && i + 1 < MIU) 
                crossover_one_point(&population[p1], &population[p2], &new_population[i], &new_population[i + 1]);

            // Se muta cada hijo
            mutate_chromosome(&new_population[i], PMU);
            decode(&new_population[i]);
            new_population[i].fitness = fitness_function(new_population[i].x);

            if (i + 1 < MIU) {
                mutate_chromosome(&new_population[i + 1], PMU);
                decode(&new_population[i + 1]);
                new_population[i + 1].fitness = fitness_function(new_population[i + 1].x);
            }
        }

        // Actualizar la población
        for (int i = 0; i < MIU; i++)
            population[i] = new_population[i];

        // Imprimir el mejor individuo de esta generación
        printf("Generacion %d \t -> Mejor x = %.2f, fitness = %.2f\n", gen, new_population[0].x, new_population[0].fitness);
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
