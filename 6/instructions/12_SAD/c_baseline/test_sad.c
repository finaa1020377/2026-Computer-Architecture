#include "sad.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

struct test_case {
    uint32_t accumulator;
    int32_t rs1;
    int32_t rs2;
    uint32_t expected;
};

static const struct test_case test_cases[] = {
    { UINT32_C(100), 10, 13, UINT32_C(103) },
    { UINT32_C(0), -10, 13, UINT32_C(23) },
    { UINT32_C(0), 13, -10, UINT32_C(23) },
    { UINT32_C(77), 42, 42, UINT32_C(77) },
    { UINT32_C(0), INT32_MAX, INT32_MIN, UINT32_C(0xffffffff) },
    { UINT32_C(0), INT32_MIN, INT32_MAX, UINT32_C(0xffffffff) },
    { UINT32_C(0xffffffff), 0, 1, UINT32_C(0) },
    { UINT32_C(0xfffffffe), INT32_MIN, 0, UINT32_C(0x7ffffffe) },
    { UINT32_C(0), -1, INT32_MIN, UINT32_C(0x7fffffff) },
    { UINT32_C(0), INT32_MAX, 0, UINT32_C(0x7fffffff) },
    { UINT32_C(1234), -200, -250, UINT32_C(1284) },
    { UINT32_C(0x80000000), 100, 13, UINT32_C(0x80000057) },
};

int main(void)
{
    size_t passed = 0;
    size_t i;

    for (i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); ++i) {
        const struct test_case *test = &test_cases[i];
        uint32_t actual = sad_accumulate(test->accumulator, test->rs1,
                                         test->rs2);

        if (actual != test->expected) {
            fprintf(stderr,
                    "test %zu failed: acc=0x%08" PRIx32
                    ", rs1=%" PRId32 ", rs2=%" PRId32
                    ", expected=0x%08" PRIx32 ", actual=0x%08" PRIx32 "\n",
                    i + 1, test->accumulator, test->rs1, test->rs2,
                    test->expected, actual);
            return 1;
        }
        ++passed;
    }

    printf("%zu tests passed.\n", passed);
    return 0;
}
