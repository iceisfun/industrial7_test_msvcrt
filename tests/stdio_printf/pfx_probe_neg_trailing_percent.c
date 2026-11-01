/* TC
 * id:    pfx_probe_neg_trailing_percent
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf format specification
 * note:  OBSERVED on ucrtbase (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "trailing r == -1 got=2"; zzp_probe2 "sprintf trailing %: r=2 errno=0 iph=0 b=[ab]"): a format ending in a lone
 *        '%' prints the text before it ("ab", r=2) and drops the '%'; no errno, no handler. The -1/EINVAL/handler belief is kept for pre-UCRT CRTs
 *        (unverified). C11 7.21.6.1p2/p9: incomplete conversion spec is UB. Stable.
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
#if TC_CRT_VER >= 140
    CHECK_EQ_I(r, 2); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* ucrtbase observed: "ab%" -> "ab" */
#else
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
#endif
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(trailing);
    return tc_end();
}
