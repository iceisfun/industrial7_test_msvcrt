/* TC
 * id:    mem_memset_int_arg
 * area:  memory
 * funcs: memset
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.6.1; MS docs memset
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_n = 8;
static volatile int v_c;

static void fill_check(int c, unsigned char expect)
{
    unsigned char b[12]; size_t i; void *r;
    memset(b, 0x5a, sizeof b);
    v_c = c;
    r = memset(b + 2, v_c, v_n);
    CHECK_EQ_P(r, b + 2);
    CHECK_EQ_U(b[0], 0x5a); CHECK_EQ_U(b[1], 0x5a);
    for (i = 2; i < 10; i++) CHECK_EQ_U(b[i], expect);
    CHECK_EQ_U(b[10], 0x5a); CHECK_EQ_U(b[11], 0x5a);
}
TC_CASE(c_0x141_fills_0x41) { fill_check(0x141, 0x41); }
TC_CASE(c_0x100_fills_zero) { fill_check(0x100, 0x00); }
TC_CASE(c_minus1_fills_0xff) { fill_check(-1, 0xff); }
TC_CASE(c_minus128_fills_0x80) { fill_check(-128, 0x80); }
TC_CASE(c_int_min_fills_zero) { fill_check((-2147483647 - 1), 0x00); }

int main(void) { tc_begin(); TC_RUN(c_0x141_fills_0x41); TC_RUN(c_0x100_fills_zero); TC_RUN(c_minus1_fills_0xff); TC_RUN(c_minus128_fills_0x80); TC_RUN(c_int_min_fills_zero); return tc_end(); }
