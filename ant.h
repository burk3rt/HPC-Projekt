#define N_CITIES 400                    //  number of cities
#define N_ANTS 128                      //  number of ants
#define N_GENERATIONS 10
#define ALPHA 1.0                    // Pheromone influence
#define BETA 5.0                     // Distance influence
#define RHO 0.5                      // Pheromone evaporation coefficient
#define QVAL 100                     // Pheromone deposit coefficient
#define INIT_PHER (1.0 / N_CITIES) // Initial pheromone level

// OpenCL config
#define N (1024*1024*64)
#define CL_PROGRAM_FILE "opencl-program.cl"
#define MAX_PLATFORMS 10
#define MAX_DEVICES 10
#define MAX_NAME_LENGTH 128

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

// Structure to represent a city
typedef struct
{
    int x; // X coordinate
    int y; // Y coordinate
} City;