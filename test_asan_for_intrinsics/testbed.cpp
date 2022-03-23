#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#if !(__ARM_NEON)
struct uint8x8_t
{
    uint8_t val[8];

    uint8_t& operator[](size_t i)
    {
        return val[i];
    }

    uint8_t const& operator[](size_t i) const
    {
        return val[i];
    }
};

uint8x8_t vld1_u8(uint8_t const* ptr)
{
    uint8x8_t r;
    for (int i = 0; i < 8; i++) {
        r[i] = ptr[i];
    }
    return r;
}

#endif

static
std::ostream& operator <<(std::ostream& os, uint8x8_t v_data)
{
    for (int i = 0; i < 8; i++) {
        if (i > 0) os << ", ";
        os << (int)v_data[i];
    }
    return os;
}

int main()
{
    uint8_t* Sp = (uint8_t*)malloc(1920);
    int xofsp[8] = {0, 1916};
    
    {
        // load and transpose 8x8 uint8_t matrix for SIMD speedup.
        uint8x8_t d0 = vld1_u8(Sp + xofsp[0]);
        uint8x8_t d1 = vld1_u8(Sp + xofsp[1]);

        std::cout << d0 << std::endl;
        std::cout << d1 << std::endl;
    }

    free(Sp);

    return 0;
}