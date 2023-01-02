#include <stdio.h>
#include <stdlib.h>

int readCsv(char *filename, int *data[], int rows) {
    FILE *fp;
    int i, j;

    fp = fopen("../../cities/data/50.csv", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < 2; j++) {
            if (fscanf(fp, "%d,", data[i][j]) != 1) {
                break;
            }
        }
    }

    fclose(fp);
    return 0;
}