#define N_CITIES 400
#define ALPHA 1.0                    // Pheromone influence
#define BETA 5.0                    // Distance influence

typedef struct
{
    int cur_city;
    int next_city;
    int start_city;
    int path_index;
    int path[N_CITIES+1];
    int visited[N_CITIES];
    double tour_length;
} Ant;

typedef struct
{
    int x;
    int y;
} City;

double clDistance(City city1, City city2);
int getRandom(int seed);

__kernel void run_ant(__constant City *cities, __global Ant *ants, __global double *phero, __constant int *seed)
{
    printf("%lu\n", getRandom(*seed));
    Ant ant_i = ants[get_global_id(0)];
    double probs[N_CITIES];
    for(int tour_steps = 0; tour_steps < N_CITIES; tour_steps++){
        if(ant_i.path_index < N_CITIES){ // If the ant has not visited all the cities
            //Calculate denominator to distribute possibility between 0 and 1
            double denominator = 0.0;
            for (int k = 0; k < N_CITIES; k++) {
                if (k != ant_i.cur_city &&
                    !ant_i.visited[k]) { // Cannot move to current city or a visited city
                    denominator += pow(phero[ant_i.cur_city * N_CITIES + k], ALPHA) * pow(1.0 / clDistance(cities[ant_i.cur_city], cities[k]), BETA);
                }
            }
            //Calculate the probabilities of moving to each city
            for (int j = 0; j < N_CITIES; j++) {
                if (j != ant_i.cur_city && !ant_i.visited[j]) { // Cannot move to current city OR already visited city
                    probs[j] = pow(phero[ant_i.cur_city * N_CITIES + j], ALPHA) *
                               pow(1.0 / clDistance(cities[ant_i.cur_city], cities[j]), BETA) / denominator;
                } else {
                    probs[j] = 0.0; // Cannot move to the current city
                }
            }
            // Choose the next city based on the probabilities
            //double r = ;
//            double total = 0.0;
//            for (int j = 0; j < N_CITIES; j++) {
//                total += probs[j];
//                if (total >= r) {
//                    ant_i.next_city = j;
//                    break;
//                }
//            }
        }
    }
}

double clDistance(City city1, City city2)
{
    double xd = city1.x - city2.x;
    double yd = city1.y - city2.y;
    return sqrt(xd * xd + yd * yd);
}

// https://stackoverflow.com/questions/9912143/how-to-get-a-random-number-in-opencl
int getRandom(int seed)
{
    ulong l_seed = seed + get_global_id(0);
    l_seed = (l_seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
    return l_seed >> 16;
}