#include "sad.h"

uint32_t sad_accumulate(uint32_t accumulator, int32_t rs1, int32_t rs2)
{
    uint32_t absolute_difference;

    if (rs1 >= rs2) {
        absolute_difference = (uint32_t)rs1 - (uint32_t)rs2;
    } else {
        absolute_difference = (uint32_t)rs2 - (uint32_t)rs1;
    }

    return accumulator + absolute_difference;
}
