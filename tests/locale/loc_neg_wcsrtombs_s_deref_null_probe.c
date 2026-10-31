/* TC
 * id:    loc_neg_wcsrtombs_s_deref_null_probe
 * area:  locale
 * funcs: 
 * procs: wcsrtombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcsrtombs_s (src validation)
 * note:  PROBE: *src == NULL (src non-NULL) is treated as an invalid parameter: EINVAL and one invalid-parameter handler call, for wcsrtombs_s. Separate from the mbsrtowcs_s variant because a CRT that does not validate *src would dereference NULL and crash.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(errno_t, wcsrtombs_s, (size_t *, char *, size_t, const wchar_t **, size_t, void *));

TC_CASE(wcs_deref_src_null)
{
    char d[8]; const wchar_t *s = NULL; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_wcsrtombs_s(&n, d, 8, &s, 4, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcsrtombs_s);
    if (!tc_p_wcsrtombs_s) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(wcs_deref_src_null);
    return tc_end();
}
