#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    double* d1 = (double*) malloc(sizeof(double));
    double* d2 = (double*) malloc(sizeof(double));
    printf("D1 heap addr: 0X%d\n", d1);
    printf("D2 heap addr: 0X%d\n", d2);
    printf("Difference: %ld\n", (long)d2 - (long)d1);
    printf("int size: %d\n", sizeof(int));
    printf("double size: %d\n", sizeof(double));
    printf("float size: %d\n", sizeof(float));
    return 0;
}
