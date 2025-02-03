#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#define NB 10            // Número de bits por individuo
#define SCALE 0.01       // Factor para pasar de binario a x real: x = (entero) * SCALE

// Definimos una estructura para representar a un individuo
typedef struct {
    int chromosome[NB];  // Cromosoma binario
    double fitness;      // Valor de fitness
    double x;            // Valor real decodificado
} Individual;

// Declaraciones de funciones
void random_chromosome(Individual *ind);
void decode(Individual *ind);
double fitness_function(double x);

#endif // INDIVIDUAL_H