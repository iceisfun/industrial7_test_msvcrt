/* TC
 * id:    mem_memset_alignment_sweep
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

static unsigned char buf[32 + 300 + 64];
static volatile size_t v_n;
static volatile int v_c;

/* returns number of mismatches; fills with c at buf+32+off, n bytes */
static int sweep_one(size_t off, size_t n, int c)
{
    size_t i, base = 16 + off, bad = 0;
    void *r;
    memset(buf, 0xa5, sizeof buf);
    v_n = n; v_c = c;
    r = memset(buf + base, v_c, v_n);
    if (r != buf + base) bad++;
    for (i = 0; i < sizeof buf; i++) {
        unsigned char want = (i >= base && i < base + n) ? (unsigned char)c : 0xa5;
        if (buf[i] != want) bad++;
    }
    return (int)bad;
}
TC_CASE(offsets_0_to_31)
{
    size_t off; int bad = 0;
    for (off = 0; off < 32; off++) bad += sweep_one(off, 77, 0x3c);
    CHECK_EQ_I(bad, 0);
}
TC_CASE(sizes_0_to_300)
{
    size_t n; int bad = 0;
    for (n = 0; n <= 300; n++) bad += sweep_one(1, n, 0x7e);
    CHECK_EQ_I(bad, 0);
}
TC_CASE(guards_intact)
{
    size_t off, n; int bad = 0;
    for (off = 0; off < 32; off += 3)
        for (n = 0; n <= 300; n += 7) bad += sweep_one(off, n, 0x00);
    CHECK_EQ_I(bad, 0);
}

int main(void) { tc_begin(); TC_RUN(offsets_0_to_31); TC_RUN(sizes_0_to_300); TC_RUN(guards_intact); return tc_end(); }
