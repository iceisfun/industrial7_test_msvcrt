/* TC
 * id:    mem_memset_zero_len
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

static volatile size_t v_zero = 0;
static volatile int v_c = 0xee;

TC_CASE(no_write)
{
    unsigned char b[16]; size_t i;
    memset(b, 0x11, sizeof b);
    memset(b + 4, v_c, v_zero);
    for (i = 0; i < sizeof b; i++) CHECK_EQ_U(b[i], 0x11);
}
TC_CASE(returns_dest)
{
    unsigned char b[16]; void *r;
    r = memset(b + 3, v_c, v_zero);
    CHECK_EQ_P(r, b + 3);
}

int main(void) { tc_begin(); TC_RUN(no_write); TC_RUN(returns_dest); return tc_end(); }
