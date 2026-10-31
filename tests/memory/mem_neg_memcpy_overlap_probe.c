/* TC
 * id:    mem_neg_memcpy_overlap_probe
 * area:  memory
 * kind:  negative
 * funcs: memcpy memmove
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.24.2.1 (overlap is UB); MS docs memcpy ("If the source and destination overlap, the behavior is undefined")
 * note:  Probes whether ucrtbase memcpy happens to be memmove-safe for 64-byte overlapping copies shifted by 8 bytes
 *        (believed true on x64: small sizes are loaded fully before being stored).
 */
#include "tc.h"
#include <string.h>

static unsigned char X[160], Y[160];
static void fill(unsigned char *p) { int i; for (i = 0; i < 160; i++) p[i] = (unsigned char)(i + 1); }

TC_CASE(dest_below_src_shift_8_size_64_equals_memmove)
{
    volatile size_t n = 64;
    fill(X); fill(Y);
    memcpy(X + 16, X + 24, n);
    memmove(Y + 16, Y + 24, n);
    CHECK_MEM(X, Y, 160);
}
TC_CASE(dest_above_src_shift_8_size_64_equals_memmove)
{
    volatile size_t n = 64;
    fill(X); fill(Y);
    memcpy(X + 24, X + 16, n);
    memmove(Y + 24, Y + 16, n);
    CHECK_MEM(X, Y, 160);
}
int main(void) { tc_begin(); TC_RUN(dest_below_src_shift_8_size_64_equals_memmove); TC_RUN(dest_above_src_shift_8_size_64_equals_memmove); return tc_end(); }
