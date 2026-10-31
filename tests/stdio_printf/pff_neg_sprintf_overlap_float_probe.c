/* TC
 * id:    pff_neg_sprintf_overlap_float_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.1p2 (overlap is undefined); MS docs sprintf_s
 * note:  PROBE: the output buffer is also a %s argument next to a %f conversion. Behaviour is undefined by C, and
 *        sprintf_s docs say overlapping buffers give undefined behaviour. We only probe that the call does not
 *        crash, returns >=0 or -1, and leaves a NUL within the 64-byte buffer. No output content is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

static int has_nul(const char *b, int n) { int i; for (i = 0; i < n; i++) if (!b[i]) return 1; return 0; }

TC_CASE(src_first)
{
    char b[64]; char *volatile p; int n;
    memset(b, 0, sizeof b); strcpy(b, "abc"); p = b;
    tc_iph_install(); if (!tc_p_sprintf_s) TC_SKIP("no sprintf_s");
    n = tc_p_sprintf_s(b, 64, "%s %.1f", p, 0.5);
    CHECK(n >= -1);
    CHECK(has_nul(b, 64));
    { char t[16]; sprintf(t, "%d", 1); /* import anchor only, unchecked */ }
}
TC_CASE(float_first)
{
    char b[64]; char *volatile p; int n;
    memset(b, 0, sizeof b); strcpy(b, "xyz"); p = b;
    tc_iph_install(); if (!tc_p_sprintf_s) TC_SKIP("no sprintf_s");
    n = tc_p_sprintf_s(b, 64, "%.1f%s", 0.5, p);
    CHECK(n >= -1);
    CHECK(has_nul(b, 64));
}

int main(void) { tc_begin(); TC_LOAD_PROC(sprintf_s); TC_RUN(src_first); TC_RUN(float_first); return tc_end(); }
