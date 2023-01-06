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
    printf("Größe Cities: %d; Index: %d\n", N_CITIES, i);
    ants[i].tour_length = 100;
}