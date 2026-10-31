/* TC
 * id:    hp_neg_calloc_zero_with_huge_other
 * area:  heap
 * kind:  negative
 * funcs: calloc free
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs calloc ("If either num or size is 0, calloc allocates a zero-length item in the heap and returns a valid pointer"); C11 7.22.3.2
 * note:  A zero factor must not be mistaken for an overflow: calloc(0,SIZE_MAX) and calloc(SIZE_MAX,0) have product 0. The contrast
 *        calloc(2,SIZE_MAX/2+1) really overflows size_t and must fail. msvcrt.dll (60) excluded: its overflow handling is undocumented.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

static volatile size_t v_a, v_b;

TC_CASE(zero_times_max)
{
    void *p; v_a = 0; v_b = SIZE_MAX;
    p = calloc(v_a, v_b);
    CHECK(p != NULL);
    free(p);
}
TC_CASE(max_times_zero)
{
    void *p; v_a = SIZE_MAX; v_b = 0;
    p = calloc(v_a, v_b);
    CHECK(p != NULL);
    free(p);
}
TC_CASE(real_overflow_contrast)
{
    void *p; v_a = 2; v_b = SIZE_MAX / 2 + 1; errno = 0;
    p = calloc(v_a, v_b);
    CHECK(p == NULL);
    if (p) free(p);
    else CHECK_EQ_I(errno, ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(zero_times_max); TC_RUN(max_times_zero); TC_RUN(real_overflow_contrast); return tc_end(); }
