/* TC
 * id:    pfx_neg_fprintf_percent_n_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (%n is disabled by default; invalid parameter handler invoked, EINVAL), _set_printf_count_output
 * note:  What partial output reaches the file is NOT asserted here (see pfx_neg_probe_fprintf_percent_n_partial_output).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_pfx_neg_fprintf_percent_n_disabled.tmp"

TC_CASE(rejected)
{
    FILE *f = fopen(FN, "w"); volatile int n = -7; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, "ab%ncd", (int *)&n);
    CHECK(r < 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(n, -7);
    tc_iph_count = 0;
    r = fprintf(f, "xyz");
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(rejected);
    return tc_end();
}
