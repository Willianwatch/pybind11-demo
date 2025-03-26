#include "alg_lib.h"

#include <stdio.h>

void print_2d_array(Frame frame)
{
    for (int i = 0; i < frame.shape[0]; i++) {
        for (int j = 0; j < frame.shape[1]; j++) {
            printf("%03d ", frame.data[i * frame.shape[1] + j]);
        }
        printf("\n");
    }
}