/* TC
 * id:    mem_memmove_basic
 * area:  memory
 * funcs: memmove
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.2; MS docs memmove
 */
#include "tc.h"
#include <string.h>

static unsigned char src[16], dst[16];
static volatile size_t v_n = 16, v_zero = 0;
static void fill(void) { size_t i; for (i = 0; i < 16; i++) { src[i] = (unsigned char)(i * 11 + 3); dst[i] = 0x55; } }

TC_CASE(copy) { fill(); memmove(dst, src, v_n); CHECK_MEM(dst, src, 16); }
TC_CASE(returns_dest) { void *volatile d = dst + 2; void *r; fill(); r = memmove(d, src, 4); CHECK_EQ_P(r, dst + 2); }
TC_CASE(zero_len_noop)
{
    unsigned char ref[16]; size_t i; void *r; fill(); for (i = 0; i < 16; i++) ref[i] = dst[i];
    r = memmove(dst, src, v_zero);
    CHECK_EQ_P(r, dst); CHECK_MEM(dst, ref, 16);
}
TC_CASE(high_bytes)
{
    static const unsigned char p[8] = { 0x00, 0xff, 0x80, 0x00, 0x7f, 0x81, 0xfe, 0x00 };
    fill(); memmove(dst, p, 8); CHECK_MEM(dst, p, 8);
    CHECK_EQ_U(dst[8], 0x55);
}
int main(void) { tc_begin(); TC_RUN(copy); TC_RUN(returns_dest); TC_RUN(zero_len_noop); TC_RUN(high_bytes); return tc_end(); }
