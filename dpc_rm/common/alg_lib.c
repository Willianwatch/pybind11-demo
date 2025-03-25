#include "alg_lib.h"

#include <stdio.h>

void print_2d_array(Frame frame)
{
    for (int i = 0; i < frame.height; i++) {
        for (int j = 0; j < frame.width; j++) {
            printf("%d ", frame.data[i * frame.width + j]);
        }
        printf("\n");
    }
}