#include <math.h>

#include "dpc_fw.h"

int dpc_fw(Frame frame, int pow_val)
{
    // for (int i = 0; i < frame.height; i++) {
    //     for (int j = 0; j < frame.width; j++) {
    //         frame.data[i * frame.width + j] = pow(frame.data[i * frame.width + j], pow_val);
    //     }
    // }    
    print_2d_array(frame);
    return 0;
}