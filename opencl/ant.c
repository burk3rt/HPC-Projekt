#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>

#include "ant.h"
#include "../cities/csv-input.c"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


void initializeAnts(Ant *ants);
double distance(City city1, City city2);
uint64_t system_current_time_millis();
cl_ulong concatenate(uint32_t x, uint32_t y);

int main(int argc, char** argv)
{
    //########################################################
    // OpenCL Stuff
    cl_platform_id platform = 0;

    // Get Devices for chosen platform
    cl_device_id *devices;
    cl_uint n_devices;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, MAX_DEVICES, NULL, &n_devices);
    devices = (cl_device_id *) malloc(n_devices * sizeof(cl_device_id));
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, n_devices, devices, NULL);
    if(n_devices == 0) {
        fprintf(stderr, "No devices found for platform. Exiting.\n");
        return -1;
    }

    char name[MAX_NAME_LENGTH];
    int choice = 2;
    printf("Devices:\n");
    for(int i=0;i<n_devices;i++) {
        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(name), name, NULL);
        printf("[%d]: %s\n", i, name);
    }
    printf("Choose device (Default %d): ", choice);
    //scanf("%d", &choice);
    if(choice < 0 || choice >= n_devices) {
        printf("Invalid choice, using device 0");
        choice = 0;
    }
    cl_device_id device;
    memcpy(&device, &devices[choice], sizeof(cl_device_id));
    free(devices);

    // Create a compute context
    cl_context context;
    context = clCreateContext(0, 1, &device, NULL, NULL, NULL);
    if (!context)
    {
        fprintf(stderr, "Failed to create a compute context.\n");
        return -1;
    }

    // Create a command queue
    cl_command_queue commands = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, NULL);
    if (!commands)
    {
        fprintf(stderr, "Failed to create a command queue.\n");
        return -1;
    }


    // Load program from file
    FILE *program_file = fopen(CL_PROGRAM_FILE, "rb");
    if(program_file == NULL) {
        fprintf(stderr, "Failed to open OpenCL program file\n");
        return -1;
    }
    fseek(program_file, 0, SEEK_END);
    size_t program_size = ftell(program_file);
    rewind(program_file);
    char *program_text = (char *) malloc((program_size + 1) * sizeof(char));
    program_text[program_size] = '\0';
    fread(program_text, sizeof(char), program_size, program_file);
    fclose(program_file);

    int err;
    cl_program program = clCreateProgramWithSource(context, 1, (const char **) &program_text, NULL, &err);
    if(err < 0)
    {
        fprintf(stderr, "Failed to create program.\n");
        return -1;
    }

    err = clBuildProgram(program, 0, NULL, "", NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        fprintf(stderr, "Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        fprintf(stderr, "%s\n", buffer);
        return -1;
    }
    free(program_text);

    // Create Kernel ... has to be adapted to program
    cl_kernel run_ant = clCreateKernel(program, "run_ant", &err);
    if (!run_ant || err != CL_SUCCESS)
    {
        fprintf(stderr, "Failed to create compute kernel!\n");
        return -1;
    }

    // Create arrays in device memory
    City cities[N_CITIES];              // Array of cities
    Ant ants[N_ANTS];                   // Array of ants
    double phero[N_CITIES*N_CITIES];   // Pheromone levels on the edges

    cl_mem d_cities = clCreateBuffer(context, CL_MEM_READ_ONLY, N_CITIES * sizeof(City), NULL, NULL);
    cl_mem d_ants = clCreateBuffer(context, CL_MEM_READ_WRITE, N_ANTS * sizeof(Ant), NULL, NULL);
    cl_mem d_phero = clCreateBuffer(context, CL_MEM_READ_ONLY, N_CITIES * N_CITIES * sizeof(double), NULL, NULL);
    cl_mem d_seed = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_ulong), NULL, NULL);

    if(!d_cities || !d_ants || !d_phero)
    {
        fprintf(stderr, "Failed to allocate memory on device\n");
        return -1;
    }

    // Set the arguments to our compute kernel
    err  = 0;
    err  = clSetKernelArg(run_ant, 0, sizeof(cl_mem), &d_cities);
    err |= clSetKernelArg(run_ant, 1, sizeof(cl_mem), &d_ants);
    err |= clSetKernelArg(run_ant, 2, sizeof(cl_mem), &d_phero);
    err |= clSetKernelArg(run_ant, 3, sizeof(cl_mem), &d_seed);
    if (err != CL_SUCCESS){
        fprintf(stderr, "Failed to set kernel arguments!\n");
        return -1;
    }

    //##################################################################
    // Ant algorithm initiation

    //double probs[N_CITIES];             // Probabilities of moving to a city

    // Find the data source depending on the number of cities
    printf("number cities=%d, number ants=%d\n", N_CITIES, N_ANTS);
    char city_amount[sizeof(N_CITIES)] = "";
    sprintf(city_amount, "%d", N_CITIES);
    char file_ending [] = ".csv";
    char basepath[18 + sizeof(city_amount) + sizeof(file_ending)] = "../cities/data/";
    strcat(basepath, city_amount);
    strcat(basepath, file_ending);

    readCitiesFromCsv(basepath, cities);

    // Initialize the pheromone levels to the initial value
    for (int i = 0; i < N_CITIES * N_CITIES; i++) {
        phero[i] = INIT_PHER;
    }

    uint64_t start = system_current_time_millis();

    // Run the ant algorithm
    for (int generation = 0; generation < N_GENERATIONS; ++generation) {
        // Initialize the ants
        initializeAnts(ants);

        // Copy data to device
        err = clEnqueueWriteBuffer(commands, d_cities, CL_TRUE, 0, N_CITIES * sizeof(City), &cities, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Failed to write to device array\n");
            return -1;
        }
        err = clEnqueueWriteBuffer(commands, d_ants, CL_TRUE, 0, N_ANTS * sizeof(Ant), &ants, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Failed to write to device array\n");
            return -1;
        }
        err = clEnqueueWriteBuffer(commands, d_phero, CL_TRUE, 0, N_CITIES * N_CITIES * sizeof(double), &phero, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Failed to write to device array\n");
            return -1;
        }
        cl_ulong seed = concatenate(rand(),rand());
        err = clEnqueueWriteBuffer(commands, d_seed, CL_TRUE, 0, sizeof(cl_ulong), &seed, 0, NULL, NULL);
        if (err != CL_SUCCESS) {
            fprintf(stderr, "Failed to write to device array\n");
            return -1;
        }
        clFinish(commands);


        // Execute the kernel
        size_t global_work_size = N_ANTS;
        size_t local_size = N_ANTS; //TODO Warum nix größer als 1?
        err = clEnqueueNDRangeKernel(commands, run_ant, 1, NULL, &global_work_size, &local_size, 0, NULL, NULL);
        if (err){
            fprintf(stderr, "Failed to execute kernel!\n");
            return -1;
        }

        // Wait for the commands to get serviced before reading back results
        clFinish(commands);

        // Read back the results from the device to verify the output
        err = clEnqueueReadBuffer( commands, d_ants, CL_TRUE, 0, N_ANTS * sizeof(Ant), ants, 0, NULL, NULL );
        if (err != CL_SUCCESS)
        {
            fprintf(stderr, "Failed to read output array\n");
            return -1;
        }
        printf("Test: %f\n", ants[5].tour_length);

//        // Returning to the start city
//        for (int i = 0; i < N_ANTS; i++)
//        {
//            ants[i].path[ants[i].path_index] = ants[i].start_city;
//            ants[i].tour_length += distance(cities[ants[i].cur_city], cities[ants[i].start_city]);
//        }
//
//        // Update the pheromone levels
//        for (int i = 0; i < N_ANTS; i++) {
//            for (int j = 1; j < N_CITIES; j++) {
//                phero[ants[i].path[j - 1]][ants[i].path[j]] += QVAL / ants[i].tour_length;
//            }
//            phero[ants[i].path[N_CITIES - 1]][ants[i].path[0]] += QVAL / ants[i].tour_length; // Return to starting city
//        }
//
//        // Evaporate pheromones
//        for (int i = 0; i < N_CITIES; i++) {
//            for (int j = 0; j < N_CITIES; j++) {
//                phero[i][j] *= (1.0 - RHO);
//            }
//        }
    }

    uint64_t end = system_current_time_millis();
    double sec = (end - start) / 1.0e3;
    printf("Time needed: %8.4f seconds\n", sec);

    return 0;




    // Time measurements
//    cl_event prof_event;
//    cl_ulong start_time, end_time;
//    size_t return_bytes;



//    clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_time, &return_bytes);
//    clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end_time, &return_bytes);
//    double time = (double) (end_time - start_time) / 1.0e9;
//    printf("Zeit: %.4f sec (nur Berechnung ohne Memory-Transfer)\nLeistung %.2f GFLOP/s\n", time, N / time / 1e9);


    // Shutdown and cleanup
    clReleaseMemObject(d_cities);
    clReleaseMemObject(d_phero);
    clReleaseMemObject(d_ants);
    clReleaseProgram(program);
    clReleaseKernel(run_ant);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

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

// Calculate the distance between two cities
double distance(City city1, City city2)
{
    int xd = city1.x - city2.x;
    int yd = city1.y - city2.y;
    return sqrt(xd * xd + yd * yd);
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

//https://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
cl_ulong concatenate(uint32_t x, uint32_t y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}