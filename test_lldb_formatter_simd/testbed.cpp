// https://github.com/intel/ARM_NEON_2_x86_SSE/blob/master/NEON_2_SSE.h
#include "NEON_2_SSE.h"
#include <stdio.h>

int main() {
    uint8_t w8u2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18};
    uint8x16_t vw8u2 = vld1q_u8(w8u2);
    uint8_t buf2[16];

    vst1q_u8(buf2, vw8u2);
    printf("Done2\n"); // set a breakpoint here

    int8_t w8u3[16] = {-128, -127, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18};
    int8x16_t vw8u3 = vld1q_s8(w8u3);
    int8_t buf3[16];
    vst1q_s8(buf3, vw8u3);
    printf("Done3\n"); // set a breakpoint here

    return 0;
}