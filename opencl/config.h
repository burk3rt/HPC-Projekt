// Ant config
#define N_CITIES 400                    //  number of cities
#define N_ANTS 32                      //  number of ants
#define N_GENERATIONS 1
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