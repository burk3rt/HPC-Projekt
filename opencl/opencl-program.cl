#define N_CITIES 400

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

__kernel void vector_add(__global const float *a, __global const float *b, __global float *c)
{
    int i = get_global_id(0);
    c[i] = a[i] + b[i];
}

__kernel void run_ant(__global City *cities, __global Ant *ants, __global double *phero)
{
    int i = get_global_id(0);
    printf("Phero: %f  Index: %d\n", phero[1* N_CITIES + 2], i);
    printf("Phero: %f  Index: %d\n", phero[2* N_CITIES + 3], i);
//    for(int j = 0; j < N_CITIES; j++){
//        for(int k = 0; k < N_CITIES; k++){
//            if(phero[j * 400 + k] != 0.002500){
//                printf("Value: %f, IndexJ: %d, IndexK: %d\n", phero[j * 400 + k], j, k);
//            }
//        }
//    }
}