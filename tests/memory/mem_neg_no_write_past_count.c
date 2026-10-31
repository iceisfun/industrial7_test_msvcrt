/* TC
 * id:    mem_neg_no_write_past_count
 * area:  memory
 * kind:  negative
 * funcs: memcpy memmove memset _memccpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1, 7.24.2.2, 7.24.6.1: only n bytes of the destination are modified; MS docs _memccpy
 * note:  For sizes 1..70 and destination offsets 0..3 (alignment variety), guard bytes either side must stay intact.
 */
#include "tc.h"
#include <string.h>

#define G 32
static unsigned char buf[G + 80 + G], srcb[96];
static volatile size_t v_n;
static volatile int v_c;

static void prep(void) { size_t i; for (i = 0; i < sizeof buf; i++) buf[i] = 0xA5; for (i = 0; i < sizeof srcb; i++) srcb[i] = (unsigned char)(i * 7 + 1); }
static int guards_ok(size_t off, size_t n)
{
    size_t i; for (i = 0; i < G + off; i++) if (buf[i] != 0xA5) return 0;
    for (i = G + off + n; i < sizeof buf; i++) if (buf[i] != 0xA5) return 0;
    return 1;
}
TC_CASE(memcpy_sizes_1_to_70_guards)
{
    size_t n, off; for (off = 0; off < 4; off++) for (n = 1; n <= 70; n++) {
        prep(); v_n = n; memcpy(buf + G + off, srcb, v_n);
        if (!guards_ok(off, n) || memcmp(buf + G + off, srcb, n)) { CHECK_EQ_U(n, 0); return; } }
}
TC_CASE(memmove_sizes_1_to_70_guards)
{
    size_t n, off; for (off = 0; off < 4; off++) for (n = 1; n <= 70; n++) {
        prep(); v_n = n; memmove(buf + G + off, srcb, v_n);
        if (!guards_ok(off, n) || memcmp(buf + G + off, srcb, n)) { CHECK_EQ_U(n, 0); return; } }
}
TC_CASE(memset_sizes_1_to_70_guards)
{
    size_t n, off, i; v_c = 0x11; for (off = 0; off < 4; off++) for (n = 1; n <= 70; n++) {
        prep(); v_n = n; memset(buf + G + off, v_c, v_n);
        if (!guards_ok(off, n)) { CHECK_EQ_U(n, 0); return; }
        for (i = 0; i < n; i++) if (buf[G + off + i] != 0x11) { CHECK_EQ_U(n, 0); return; } }
}
TC_CASE(memccpy_sizes_1_to_70_guards)
{
    size_t n, off; void *r;  /* 0xEE never occurs in srcb[0..69] (index 107 would) */
    for (off = 0; off < 4; off++) for (n = 1; n <= 70; n++) {
        size_t i; int found = 0; for (i = 0; i < n; i++) if (srcb[i] == 0xEE) found = 1;
        prep(); v_n = n; r = _memccpy(buf + G + off, srcb, 0xEE, v_n);
        if (found) { CHECK(r != NULL); break; }
        if (r != NULL || !guards_ok(off, n) || memcmp(buf + G + off, srcb, n)) { CHECK_EQ_U(n, 0); return; } }
    /* stops at c: copy through first 'c' only */
    prep(); srcb[5] = 0x42; v_n = 70; r = _memccpy(buf + G, srcb, 0x42, v_n);
    CHECK(r == buf + G + 6);
    CHECK(guards_ok(0, 6));
}
int main(void)
{
    tc_begin();
    TC_RUN(memcpy_sizes_1_to_70_guards); TC_RUN(memmove_sizes_1_to_70_guards);
    TC_RUN(memset_sizes_1_to_70_guards); TC_RUN(memccpy_sizes_1_to_70_guards);
    return tc_end();
}
