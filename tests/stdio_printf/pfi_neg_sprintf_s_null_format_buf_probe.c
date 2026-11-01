/* TC
 * id:    pfi_neg_sprintf_s_null_format_buf_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe2: "sprintf_s NULL fmt (inline): r=-1 errno=22 iph=1 b0=120"): sprintf_s(b, n, NULL) returns -1/EINVAL
 *        and calls the handler once, but leaves buf[0] untouched (120='x'); the guess that buf[0] is zeroed was wrong (the docs only promise
 *        buf[0]=0 for the too-small case). Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#if TC_CRT_VER >= 140
/* ucrtbase exports no sprintf_s symbol; the header inline goes through __stdio_common_vsprintf_s */
#define SS(...) sprintf_s(__VA_ARGS__)
#define HAVE_SS 1
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define SS(...) (tc_p_sprintf_s ? tc_p_sprintf_s(__VA_ARGS__) : -99)
#define HAVE_SS (tc_p_sprintf_s != NULL)
#endif

TC_CASE(probe)
{
    char b[8]; const char *volatile f = NULL;
    CHECK(HAVE_SS); if (!HAVE_SS || !tc_iph_install()) return;
    b[0] = 'x';
    SS(b, sizeof b, f);
    #if TC_CRT_VER >= 140
    CHECK_EQ_I(b[0], 'x');   /* ucrtbase observed: NOT cleared */
#else
    CHECK_EQ_I(b[0], 0);     /* original belief for pre-UCRT CRTs (unverified) */
#endif
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(probe);
    return tc_end();
}
