/* TC
 * id:    pfs_neg_probe_vsnprintf_null_count0
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf (count 0 / NULL buffer)
 * note:  PROBE: _snprintf(NULL, 0, "abc") either returns -1 (legacy: nothing can be stored) or 3 (C99-style length query), and
 *        must not crash. The invalid-parameter count is only reported, not asserted.
 */
#include "tc.h"
#include <stdio.h>

static char * volatile nullbuf = NULL;
static const char * volatile fmt = "abc";

TC_CASE(null_count0)
{
    char n[16]; int r, have = tc_iph_install();
    tc_iph_count = 0;
    r = _snprintf(nullbuf, 0, fmt);
    CHECK(r == -1 || r == 3);
    tc_write("TC INFO ret="); tc_i64(r, n); tc_write(n);
    tc_write(" iph="); tc_i64(have ? tc_iph_count : -1, n); tc_write(n); tc_write("\n");
}
int main(void) { tc_begin(); TC_RUN(null_count0); return tc_end(); }
