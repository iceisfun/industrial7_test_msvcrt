/* TC
 * id:    pfp_pct_width_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1 (only %% is defined); MS docs printf type field
 * note:  PROBE: decorated percent forms (%5% etc) are undefined in C. Asserted loosely: ret<0, or strlen(output) equals the return value (no crash, consistent count). Whether a percent is emitted is NOT asserted (undefined).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void chk(const char *fmt)
{
    char b[64]; int n, i, has = 0; size_t l = 0;
    for (i = 0; i < 64; i++) b[i] = 0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = sprintf(b, fmt);
    if (n < 0) return;                 /* rejected: acceptable */
    while (b[l]) { if (b[l] == '%') has = 1; l++; }
    CHECK_EQ_U(l, n);
}
static const char *volatile F1 = "%5%", *volatile F2 = "%-5%|", *volatile F3 = "%05%", *volatile F4 = "%.2%";
TC_CASE(w5)    { chk(F1); }
TC_CASE(left)  { chk(F2); }
TC_CASE(zero)  { chk(F3); }
TC_CASE(prec)  { chk(F4); }

int main(void) { tc_begin(); TC_RUN(w5); TC_RUN(left); TC_RUN(zero); TC_RUN(prec); return tc_end(); }
