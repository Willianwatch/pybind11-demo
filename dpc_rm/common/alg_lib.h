#ifndef __ALG_LIB_H__
#define __ALG_LIB_H__

#define uint unsigned int
#define ushort unsigned short
#define uchar unsigned char
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))

typedef struct
{
    int width;
    int height;
    ushort *data;
    int shape[2];
} Frame;

void print_2d_array(Frame frame);

#endif