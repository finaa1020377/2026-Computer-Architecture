#include "sad.h"

uint32_t sad_accumulate(uint32_t accumulator, int32_t rs1, int32_t rs2)
{
    /* d = rs1 - rs2, computed modulo 2^32 like the hardware subtractor. */
    uint32_t difference = (uint32_t)rs1 - (uint32_t)rs2;
    uint32_t absolute_difference;

    /* |d|: negate when the sign bit is set; -0x80000000 stays 0x80000000. */
    if ((difference & UINT32_C(0x80000000)) != 0) {
        absolute_difference = UINT32_C(0) - difference;
    } else {
        absolute_difference = difference;
    }

    /* The accumulator also wraps modulo 2^32. */
    return accumulator + absolute_difference;
}
