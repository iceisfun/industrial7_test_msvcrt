/* TC
 * id:    pfr_neg_probe_sprintf_s_null_format_buf0
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p2
 * conf:  observed
 * ref:   MS docs sprintf_s
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "b[0] == 0 got=88"; zzp_probe2 "sprintf_s NULL fmt (inline) b0=120"): after sprintf_s(b, n, NULL) fails with
 *        EINVAL, b[0] is left unchanged (not zeroed). Original guess wrong; docs promise buf[0]=0 only for the too-small buffer case. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;

/* msvcr80's import library lacks the _s functions: resolve non-UCRT targets at run time (ucrtbase has them inline). */
#if TC_CRT_VER < 140
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
#endif
TC_CASE(buf0_after_null_format)
{
    char b[8]; const char *volatile f = NULL;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b);
    sprintf_s(b, sizeof b, f);
    #if TC_CRT_VER >= 140
    CHECK_EQ_I(b[0], 'X');   /* ucrtbase observed: NOT cleared */
#else
    CHECK_EQ_I(b[0], 0);     /* original belief for pre-UCRT CRTs (unverified) */
#endif
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(buf0_after_null_format);
    return tc_end();
}
