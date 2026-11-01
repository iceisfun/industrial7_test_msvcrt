/* TC
 * id:    loc_neg_mbsrtowcs_s_deref_null_probe
 * area:  locale
 * funcs: 
 * procs: mbsrtowcs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcsrtombs_s (src validation)
 * note:  Win10 triage (class B, probe -> observed): ucrtbase 19045 returns 0 (not EINVAL), *pReturnValue 1, d[0] == 0, no invalid-parameter call. Observed on
 *        Windows 10 22H2 build 19045 (the wcsrtombs_s counterpart crashes instead, see loc_neg_wcsrtombs_s_deref_null_probe).
 *        Original PROBE: *src == NULL (src non-NULL) is treated as an invalid parameter: EINVAL and one invalid-parameter handler call, for mbsrtowcs_s. Separate from the wcsrtombs_s variant because a CRT that does not validate *src would dereference NULL and crash.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [class B] deviates from the runtime-constraint rules of C11 Annex K (K.3.6.x / K.3.9.3.x) that the MS docs mirror; expectation = Windows behavior.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(errno_t, mbsrtowcs_s, (size_t *, wchar_t *, size_t, const char **, size_t, void *));

TC_CASE(mbs_deref_src_null)
{
    wchar_t d[8]; const char *s = NULL; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    { int k; for (k = 0; k < 8; k++) d[k] = 0x5555; }
    c0 = tc_iph_count; errno = 0;
    e = tc_p_mbsrtowcs_s(&n, d, 8, &s, 4, PST);
    /* Win10 22H2 ucrtbase (observed): *src == NULL is not validated for mbsrtowcs_s: returns 0, *pReturnValue = 1, writes an empty string, no handler. */
    CHECK_EQ_I(e, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count - c0, 0);
    CHECK_EQ_U(n, 1); CHECK_EQ_U(d[0], 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs_s);
    if (!tc_p_mbsrtowcs_s) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_deref_src_null);
    return tc_end();
}
