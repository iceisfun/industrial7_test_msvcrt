/* TC
 * id:    pfx_probe_neg_trailing_percent
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf format specification
 * note:  Probing -1 / EINVAL / iph count 1 for a format ending in a lone '%'.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_trailing_percent_" TCV ".tmp"
static const char *volatile g_fmt = "ab%";

TC_CASE(trailing)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(trailing);
    return tc_end();
}
