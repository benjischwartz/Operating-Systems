#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int x;
    int* x_ptr = &x;
    x_ptr = NULL;
    printf("Dererencing null pointer... %d", *x_ptr);
}
