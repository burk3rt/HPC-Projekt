#include "ant.h"
#include "../cities/5/cities5.h"
#include "../cities/csv-input.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include <stdint.h>
#include <omp.h>


#define N_CITIES 400                    //  number of cities
#define N_ANTS 128                      //  number of ants
#define N_GENERATIONS 10
#define ALPHA 1.0                    // Pheromone influence
#define BETA 5.0                     // Distance influence
#define RHO 0.5                      // Pheromone evaporation coefficient
#define QVAL 100                     // Pheromone deposit coefficient
#define INIT_PHER (1.0 / N_CITIES) // Initial pheromone level


// Structure to represent an ant
typedef struct
{
    int cur_city;         // Current city
    int next_city;        // Next city to visit
    int start_city;       // Start city
    int path_index;       // Index in the path array
    int path[N_CITIES+1];     // Visited cities in order
    int visited[N_CITIES+1]; // Array to keep track of visited cities
    double tour_length;   // Length of current tour
} Ant;



void initializeAnts(Ant *ants);
uint64_t system_current_time_millis();

// Calculate the distance between two cities
double distance(City city1, City city2)
{
    int xd = city1.x - city2.x;
    int yd = city1.y - city2.y;
    return sqrt(xd * xd + yd * yd);
}

int main() {
    City cities[N_CITIES];              // Array of cities
    Ant ants[N_ANTS];                   // Array of ants
    double phero[N_CITIES][N_CITIES];   // Pheromone levels on the edges

    // Read in the number of cities and ants
    printf("OPEN_MP\n");
    printf("number cities=%d, number ants=%d\n", N_CITIES, N_ANTS);
    char city_amount[sizeof(N_CITIES)] = "";
    sprintf(city_amount, "%d", N_CITIES);
    char file_ending [] = ".csv";
    char basepath[18 + sizeof(city_amount) + sizeof(file_ending)] = "../cities/data/";
    strcat(basepath, city_amount);
    strcat(basepath, file_ending);
    //printf("%s", basepath);

    int data[N_CITIES][2];
    readCitiesFromCsv(basepath, cities);

    // Initialize the pheromone levels to the initial value
    for (int i = 0; i < N_CITIES; i++) {
        for (int j = 0; j < N_CITIES; j++) {
            phero[i][j] = INIT_PHER;
        }
    }

    // Zeitmessen Start
    uint64_t start = system_current_time_millis();

    // Run the ant algorithm
    for (int generation = 0; generation < N_GENERATIONS; ++generation) {
        // Initialize the ants
        initializeAnts(ants);

        #pragma omp parallel for
        // Have each ant explore the solution space
        for (int i = 0; i < N_ANTS; i++) {
            for (int tour_steps = 0; tour_steps < N_CITIES; ++tour_steps) {
                if (ants[i].path_index < N_CITIES) { // If the ant has not visited all the cities
                    //Calculate denominator to distribute possibility between 0 and 1
                    double denominator = 0.0;
                    for (int k = 0; k < N_CITIES; k++) {
                        if (k != ants[i].cur_city &&
                            !ants[i].visited[k]) { // Cannot move to current city or a visited city
                            denominator += pow(phero[ants[i].cur_city][k], ALPHA) *
                                           pow(1.0 / distance(cities[ants[i].cur_city], cities[k]), BETA);
                        }
                    }
                    // Calculate the probabilities of moving to each city
                    double probs[N_CITIES];             // Probabilities of moving to a city     // Number of ants
                    for (int j = 0; j < N_CITIES; j++) {
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
                    for (int j = 0; j < N_CITIES; j++) {
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
        }
        // Returning to the start city
        for (int i = 0; i < N_ANTS; i++)
        {
            ants[i].path[ants[i].path_index] = ants[i].start_city;
            ants[i].tour_length += distance(cities[ants[i].cur_city], cities[ants[i].start_city]);
        }

        // Update the pheromone levels
        for (int i = 0; i < N_ANTS; i++) {
            for (int j = 1; j < N_CITIES; j++) {
                phero[ants[i].path[j - 1]][ants[i].path[j]] += QVAL / ants[i].tour_length;
            }
            phero[ants[i].path[N_CITIES - 1]][ants[i].path[0]] += QVAL / ants[i].tour_length; // Return to starting city
        }

        // Evaporate pheromones
        for (int i = 0; i < N_CITIES; i++) {
            for (int j = 0; j < N_CITIES; j++) {
                phero[i][j] *= (1.0 - RHO);
            }
        }
    }
    // Finished the ant algorithm



    // Find the shortest tour
    int min_index = 0;
    double min_length = ants[0].tour_length;
    for (int i = 1; i < N_ANTS; i++)
    {
        if (ants[i].tour_length < min_length)
        {
            min_length = ants[i].tour_length;
            min_index = i;
        }
    }

    // Zeitmessen Ende
    uint64_t end = system_current_time_millis();
    double sec = (end - start) / 1.0e3;

    // check if all cities were visited
    int visited_cities = 0;
    for (int k = 0; k < N_CITIES; ++k) {
        if(ants[min_index].visited[k])
            visited_cities++;
    }
    printf("%d Cities\n", visited_cities);
    // Print the shortest tour
    printf("Shortest tour: ");
    for (int i = 0; i <= N_CITIES; i++)
    {
        printf("%d ", ants[min_index].path[i]);
        if(i == 4){
            i = N_CITIES - 5;
            printf(" ... ");
        }
    }
    printf("\nTour length: %lf\n", ants[min_index].tour_length);
    printf("Time needed for algorithm to run: %8.4f seconds\n", sec);

    return 0;
}

void initializeAnts(Ant *ants) {
    for (int i = 0; i < N_ANTS; i++) {
        ants[i].cur_city = rand() % N_CITIES; // Assign a random starting city
        ants[i].start_city = ants[i].cur_city;
        ants[i].path_index = 0;               // Set the path index to 0
        ants[i].tour_length = 0;              // Set the tour length to 0
        memset(ants[i].visited, 0, sizeof ants[i].visited);
    }
}

uint64_t system_current_time_millis()
{
#if defined(_WIN32) || defined(_WIN64)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
}
