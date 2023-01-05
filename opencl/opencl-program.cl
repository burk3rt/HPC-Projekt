typedef struct
{
    int cur_city;
    int next_city;
    int start_city;
    int path_index;
    int path[400+1];
    int visited[400+1];
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
    ants[i].tour_length = 100;
}