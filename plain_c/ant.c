#include "ant.h"
#include "../cities/5/cities5.h"
#include "../cities/10/cities10.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../cities/5/cities5.h"

#define CITIES 10                    //  number of cities
#define ANTS 25                      //  number of ants
#define ALPHA 1.0                    // Pheromone influence
#define BETA 5.0                     // Distance influence
#define RHO 0.5                      // Pheromone evaporation coefficient
#define QVAL 100                     // Pheromone deposit coefficient
#define INIT_PHER (1.0 / CITIES) // Initial pheromone level

// Structure to represent an ant
typedef struct
{
    int cur_city;         // Current city
    int next_city;        // Next city to visit
    int start_city;       // Start city
    int path_index;       // Index in the path array
    int path[CITIES]; // Visited cities in order
    int visited[CITIES]; // Array to keep track of visited cities
    double tour_length;   // Length of current tour
} Ant;

// Calculate the distance between two cities
double distance(City city1, City city2)
{
    int xd = city1.x - city2.x;
    int yd = city1.y - city2.y;
    return sqrt(xd * xd + yd * yd);
}

int main() {
    City cities[CITIES];              // Array of cities
    Ant ants[ANTS];                   // Array of ants
    double phero[CITIES][CITIES]; // Pheromone levels on the edges
    double probs[CITIES];             // Probabilities of moving to a city     // Number of ants
    int i, j;                             // Loop indices

    // Read in the number of cities and ants
    printf("number cities=%d, number ants=%d\n", CITIES, ANTS);

    getCities10(cities);

    // Initialize the pheromone levels to the initial value
    for (i = 0; i < CITIES; i++) {
        for (j = 0; j < CITIES; j++) {
            phero[i][j] = INIT_PHER;
        }
    }

    // Initialize the ants
    for (i = 0; i < ANTS; i++) {
        ants[i].cur_city = rand() % CITIES; // Assign a random starting city
        ants[i].start_city = ants[i].cur_city;
        ants[i].path_index = 0;               // Set the path index to 0
        ants[i].tour_length = 0;              // Set the tour length to 0
    }

    // Run the ant algorithm
    int tour_steps = 0;
    while (tour_steps < CITIES) {
        // Have each ant explore the solution space
        for (i = 0; i < ANTS; i++) {
            if (ants[i].path_index < CITIES) { // If the ant has not visited all the cities
                //Calculate denominator to distribute possibility between 0 and 1
                double denominator = 0.0;
                for (int k = 0; k < CITIES; k++) {
                    if (k != ants[i].cur_city &&
                        !ants[i].visited[k]) { // Cannot move to current city or a visited city
                        denominator += pow(phero[ants[i].cur_city][k], ALPHA) *
                                       pow(1.0 / distance(cities[ants[i].cur_city], cities[k]), BETA);
                    }
                }
                // Calculate the probabilities of moving to each city
                for (j = 0; j < CITIES; j++) {
                    if (j != ants[i].cur_city && !ants[i].visited[j]) { // Cannot move to current city OR already visited city
                        probs[j] = pow(phero[ants[i].cur_city][j], ALPHA) *
                                   pow(1.0 / distance(cities[ants[i].cur_city], cities[j]), BETA) / denominator;
                    } else {
                        probs[j] = 0.0; // Cannot move to the current city
                    }
                }
                // Choose the next city based on the probabilities
                double r = ((double) rand()) / RAND_MAX;
                double total = 0.0;
                for (j = 0; j < CITIES; j++) {
                    total += probs[j];
                    if (total >= r) {
                        ants[i].next_city = j;
                        break;
                    }
                }
                // Update the ant's current city and path
                ants[i].visited[ants[i].cur_city] = 1;
                ants[i].path[ants[i].path_index++] = ants[i].cur_city;
                ants[i].tour_length += distance(cities[ants[i].cur_city], cities[ants[i].next_city]);
                ants[i].cur_city = ants[i].next_city;
            }
        }
        // Update the pheromone levels
        for (i = 0; i < ANTS; i++) {
            for (j = 1; j < CITIES; j++) {
                phero[ants[i].path[j - 1]][ants[i].path[j]] += QVAL / ants[i].tour_length;
            }
            phero[ants[i].path[CITIES - 1]][ants[i].path[0]] += QVAL / ants[i].tour_length; // Return to starting city
        }

        // Evaporate pheromones
        for (i = 0; i < CITIES; i++) {
            for (j = 0; j < CITIES; j++) {
                phero[i][j] *= (1.0 - RHO);
            }
        }
        tour_steps += 1;
    }
    // Finished the ant algorithm

    // Returning to the start city
    for (i = 0; i < ANTS; i++)
    {
        ants[i].path[ants[i].path_index++] = ants[i].start_city;
        ants[i].tour_length += distance(cities[ants[i].cur_city], cities[ants[i].start_city]);
    }

    // Find the shortest tour
    int min_index = 0;
    double min_length = ants[0].tour_length;
    for (i = 1; i < ANTS; i++)
    {
        if (ants[i].tour_length < min_length)
        {
            min_length = ants[i].tour_length;
            min_index = i;
        }
    }
    // Print the shortest tour
    printf("Shortest tour: ");
    for (i = 0; i <= CITIES; i++)
    {
        printf("%d ", ants[min_index].path[i]);
    }
    printf("\nTour length: %lf\n", ants[min_index].tour_length);
    return 0;
}