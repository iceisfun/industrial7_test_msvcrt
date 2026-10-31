/* TC
 * id:    mem_memmove_same_ptr
 * area:  memory
 * funcs: memmove
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.2
 */
#include "tc.h"
#include <string.h>

static unsigned char big[4096];
static volatile size_t v_n;

TC_CASE(same_small)
{
    unsigned char b[16], ref[16]; size_t i; unsigned char *volatile p = b;
    for (i = 0; i < 16; i++) { b[i] = (unsigned char)(i + 0x80); ref[i] = b[i]; }
    v_n = 16; memmove(p, p, v_n); CHECK_MEM(b, ref, 16);
}
TC_CASE(same_large_4k)
{
    size_t i, bad = 0; unsigned char *volatile p = big;
    for (i = 0; i < 4096; i++) big[i] = (unsigned char)(i * 3 + (i >> 8));
    v_n = 4096; memmove(p, p, v_n);
    for (i = 0; i < 4096; i++) if (big[i] != (unsigned char)(i * 3 + (i >> 8))) bad++;
    CHECK_EQ_U(bad, 0);
}
TC_CASE(returns_dest) { unsigned char *volatile p = big + 100; v_n = 50; CHECK_EQ_P(memmove(p, p, v_n), big + 100); }
int main(void) { tc_begin(); TC_RUN(same_small); TC_RUN(same_large_4k); TC_RUN(returns_dest); return tc_end(); }
