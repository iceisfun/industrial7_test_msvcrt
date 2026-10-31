/* TC
 * id:    mem_memcpy_alignment
 * area:  memory
 * funcs: memcpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1
 */
#include "tc.h"
#include <string.h>

/* Layout: [guard 64][region][guard 64]; pattern of source is position-dependent. */
#define G 64
#define MAXN 40
static unsigned char sbuf[G + 16 + MAXN + G], dbuf[G + 16 + MAXN + G];
static volatile size_t v_n;

static void prep(void)
{
    size_t i;
    for (i = 0; i < sizeof sbuf; i++) sbuf[i] = (unsigned char)(i * 13 + 5);
    for (i = 0; i < sizeof dbuf; i++) dbuf[i] = 0xcc;
}
/* returns 0 on success; checks copied bytes and that every other byte is still 0xcc */
static int run(size_t so, size_t doff, size_t n)
{
    size_t i; prep(); v_n = n;
    memcpy(dbuf + G + doff, sbuf + G + so, v_n);
    for (i = 0; i < sizeof dbuf; i++) {
        unsigned char want = 0xcc;
        if (i >= G + doff && i < G + doff + n) want = sbuf[G + so + (i - G - doff)];
        if (dbuf[i] != want) return 1;
    }
    return 0;
}
TC_CASE(src_off_sweep) { size_t o, n, bad = 0; for (o = 0; o < 16; o++) for (n = 1; n <= 33; n += 4) bad += run(o, 0, n); CHECK_EQ_U(bad, 0); }
TC_CASE(dst_off_sweep) { size_t o, n, bad = 0; for (o = 0; o < 16; o++) for (n = 1; n <= 33; n += 4) bad += run(0, o, n); CHECK_EQ_U(bad, 0); }
TC_CASE(both_off_sweep_sizes_1_to_40)
{
    size_t so, d, n, bad = 0;
    for (so = 0; so < 16; so++) for (d = 0; d < 16; d++) for (n = 1; n <= MAXN; n++) bad += run(so, d, n);
    CHECK_EQ_U(bad, 0);
}
TC_CASE(guards_intact)
{
    size_t i, bad = 0; run(3, 5, 40);
    for (i = 0; i < G + 5; i++) if (dbuf[i] != 0xcc) bad++;
    for (i = G + 5 + 40; i < sizeof dbuf; i++) if (dbuf[i] != 0xcc) bad++;
    CHECK_EQ_U(bad, 0);
}
int main(void) { tc_begin(); TC_RUN(src_off_sweep); TC_RUN(dst_off_sweep); TC_RUN(both_off_sweep_sizes_1_to_40); TC_RUN(guards_intact); return tc_end(); }
