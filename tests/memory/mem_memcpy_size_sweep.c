/* TC
 * id:    mem_memcpy_size_sweep
 * area:  memory
 * funcs: memcpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1
 */
#include "tc.h"
#include <string.h>

#define G 32
#define MAXN 4200
static unsigned char sbuf[MAXN + 8], dbuf[G + MAXN + G];
static volatile size_t v_n;

static int one(size_t n)
{
    size_t i;
    for (i = 0; i < sizeof sbuf; i++) sbuf[i] = (unsigned char)((i * 31 + 7) ^ (i >> 8));
    for (i = 0; i < sizeof dbuf; i++) dbuf[i] = 0xcc;
    v_n = n;
    memcpy(dbuf + G, sbuf + 1, v_n);
    for (i = 0; i < sizeof dbuf; i++) {
        unsigned char want = 0xcc;
        if (i >= G && i < G + n) want = sbuf[1 + i - G];
        if (dbuf[i] != want) return 1;
    }
    return 0;
}
TC_CASE(sizes_0_to_300) { size_t n, bad = 0, first = 0; for (n = 0; n <= 300; n++) if (one(n)) { if (!bad) first = n; bad++; } CHECK_EQ_U(bad, 0); CHECK_EQ_U(first, 0); }
TC_CASE(pow2_boundaries)
{
    static const size_t sz[] = { 511, 512, 513, 1023, 1024, 1025, 4095, 4096, 4097 };
    size_t i, bad = 0; for (i = 0; i < sizeof sz / sizeof sz[0]; i++) bad += one(sz[i]);
    CHECK_EQ_U(bad, 0);
}
TC_CASE(guards_before_after_intact)
{
    size_t i, bad = 0; one(4097);
    for (i = 0; i < G; i++) if (dbuf[i] != 0xcc) bad++;
    for (i = G + 4097; i < sizeof dbuf; i++) if (dbuf[i] != 0xcc) bad++;
    CHECK_EQ_U(bad, 0);
}
int main(void) { tc_begin(); TC_RUN(sizes_0_to_300); TC_RUN(pow2_boundaries); TC_RUN(guards_before_after_intact); return tc_end(); }
