/* TC
 * id:    loc_neg_wcsrtombs_s_deref_null_probe
 * area:  locale
 * funcs: 
 * procs: wcsrtombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcsrtombs_s (src validation); observed Windows 10 22H2 behaviour
 * note:  Win10 triage (class C): on ucrtbase 19045 wcsrtombs_s(&n, d, 8, &s, 4, &st) with s == NULL (src itself non-NULL) is not validated:
 *        it dereferences *src and dies with STATUS_ACCESS_VIOLATION (0xC0000005). The crash is the only behaviour, so the call runs in a
 *        child process and the parent asserts the exit status. (The mbsrtowcs_s counterpart returns 0 without crashing, see
 *        loc_neg_mbsrtowcs_s_deref_null_probe.) Observed on Windows 10 22H2 build 19045.
 *        [scope] the access violation is observed on ucrtbase 19045 only (TC_AT_LEAST(140)); legacy CRTs keep the original EINVAL+handler
 *        expectation, [unverified] until the P3 runs.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
TC_DECL_PROC(errno_t, wcsrtombs_s, (size_t *, char *, size_t, const wchar_t **, size_t, void *));
#if TC_AT_LEAST(140)
TC_CASE(wcs_deref_src_null_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("deref_null", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
#else
TC_CASE(wcs_deref_src_null)
{
    char d[8]; const wchar_t *s = NULL; size_t n = 99; errno_t e; int c0; memset(&st, 0, sizeof st);
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_wcsrtombs_s(&n, d, 8, &s, 4, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
}
#endif
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcsrtombs_s);
#if TC_AT_LEAST(140)
    if (tc_child_name() != NULL) {
        char d[8]; const wchar_t *s = NULL; size_t n = 99;
        if (!tc_p_wcsrtombs_s) return 78;
        memset(&st, 0, sizeof st); tc_iph_install();
        tc_p_wcsrtombs_s(&n, d, 8, &s, 4, PST);
        return 77;   /* reached only if the CRT did not crash */
    }
#endif
    if (!tc_p_wcsrtombs_s) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
#if TC_AT_LEAST(140)
    TC_RUN(wcs_deref_src_null_access_violation);
#else
    TC_RUN(wcs_deref_src_null);
#endif
    return tc_end();
}
