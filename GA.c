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

// Función objetivo a maximizar: f(x) = 30 - (x-5)^2 
double fitness_function(double x) {
    return 30.0 - (x - 5.0) * (x - 5.0);
}

/* Genera un cromosoma binario aleatorio */
void random_chromosome(int chrom[NB]) {
    for (int i = 0; i < NB; i++)
        chrom[i] = rand() % 2;
}

/* Decodifica un cromosoma binario a un valor real */
double decode(const int chrom[NB]) {
    double value = 0.0;
    double power_of_2 = 1.0; // 2^0

    for (int i = NB - 1; i >= 0; i--) { // Recorre el cromosoma de derecha a izquierda
        if (chrom[i] == 1)
            value += power_of_2;
        power_of_2 *= 2.0;
    }
    return value * SCALE;
}

/* Aplica la selección por ruleta para obtener un índice
   de individuo en [0, MIU-1] */
int roulette_selection(const double *fitness, double sum_fit) {
    double r = ((double)rand() / RAND_MAX) * sum_fit;
    double acumulado = 0.0;
    for (int i = 0; i < MIU; i++) {
        acumulado += fitness[i];
        if (acumulado >= r)
            return i;
        
    }
    // Por si hay error numérico
    return MIU - 1;
}

/* Cruce de un punto entre dos cromosomas parent1 y parent2,
   generando dos hijos child1 y child2 */
void crossover_one_point(const int parent1[NB], const int parent2[NB], int child1[NB], int child2[NB]) {
    // Selecciona aleatoriamente un punto de cruce
    // El rango va de 1 a NB-1 para no cruzar en los extremos
    int point = rand() % (NB - 1) + 1;

    // Copiamos la parte inicial
    for (int i = 0; i < point; i++) {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }
    // Copiamos la parte final
    for (int i = point; i < NB; i++) {
        child1[i] = parent2[i];
        child2[i] = parent1[i];
    }
    //printf("Cruce en el punto %d\n", point);
}

/* Mutación bit a bit con probabilidad pmu */
void mutate_chromosome(int chrom[NB], double pmu) {
    for (int i = 0; i < NB; i++) {
        double r = (double)rand() / RAND_MAX;
        if (r < pmu)
            chrom[i] = !chrom[i];  // Invierte el bit
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));

    int population[MIU][NB]; // Población inicial
    double fitness[MIU]; // Fitness de la población

    for (int i = 0; i < MIU; i++) 
        random_chromosome(population[i]);

    double sum_fitness = 0.0;
    for (int i = 0; i < MIU; i++) {
        double x = decode(population[i]);
        fitness[i] = fitness_function(x);
        sum_fitness += fitness[i];  
    }

    for (int gen = 1; gen <= T; gen++) {

        sum_fitness = 0.0;
        for (int i = 0; i < MIU; i++)
            sum_fitness += fitness[i];

        int new_population[MIU][NB]; // Nueva población
        double new_fitness[MIU]; // Fitness de la nueva población

        for (int i = 0; i < MIU; i += 2) {
            // Seleccionar dos padres por ruleta
            int p1 = roulette_selection(fitness, sum_fitness);
            int p2;
            do {
                p2 = roulette_selection(fitness, sum_fitness);
            } while (p1 == p2);


            //printf("Padres: %f %f\n", p1*SCALE, p2*SCALE);

            // Copiamos por defecto los padres como hijos
            // (por si no hay cruce)
            for (int b = 0; b < NB; b++) {
                new_population[i][b]   = population[p1][b];
                new_population[i+1][b] = population[p2][b];
            }

            // Decidir si se realiza cruce
            double rc = (double)rand() / RAND_MAX;
            if (rc < PCR) 
                crossover_one_point(population[p1], population[p2], new_population[i], new_population[i+1]);

            // Se muta cada hijo
            mutate_chromosome(new_population[i], PMU);
            mutate_chromosome(new_population[i+1], PMU);
        }

        for (int i = 0; i < MIU; i++) {
            double x = decode(new_population[i]);
            new_fitness[i] = fitness_function(x);
        }
        for (int i = 0; i < MIU; i++) {
            for (int b = 0; b < NB; b++)
                population[i][b] = new_population[i][b];
            fitness[i] = new_fitness[i];
        }

        double best_fit = fitness[0];
        int best_idx = 0;
        for (int i = 1; i < MIU; i++) 
            if (fitness[i] > best_fit) {
                best_fit = fitness[i];
                best_idx = i;
            }

        double best_x = decode(population[best_idx]);
        printf("Generacion %d \t -> Mejor x = %.2f, fitness = %.2f\n", gen, best_x, best_fit);
    }

    double best_fit = fitness[0];
    int best_idx = 0;
    for (int i = 1; i < MIU; i++) 
        if (fitness[i] > best_fit) {
            best_fit = fitness[i];
            best_idx = i;
        }

    double best_x = decode(population[best_idx]);

    printf("\nMejor individuo tras %d generaciones:\n", T);
    printf("  Cromosoma: ");
    for (int b = 0; b < NB; b++) 
        printf("%d", population[best_idx][b]);
    
    printf("\n  x = %.2f\n", best_x);
    printf("  Fitness = %.2f\n", best_fit);

    return 0;
}
