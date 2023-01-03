#include "../plain_c/ant.h"
#include <stdio.h>
#include <stdlib.h>

int readCitiesFromCsv(char *filename, City *cities) {
    FILE *fp;
    int i, j;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int count = 0;
    while (fscanf(fp, " %d,%d", &cities[count].x, &cities[count].y) == 2) {
        count++;
    }

    fclose(fp);
    return 0;
}