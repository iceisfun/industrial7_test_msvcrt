/* TC
 * id:    wcs_neg_probe_s_null_dest_no_iph_when_size0_nullsrc
 * area:  wstring
 * funcs:
 * procs: wcscpy_s wcscat_s wcsncpy_s wcsncat_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcscpy_s / wcsncpy_s / wcscat_s / wcsncat_s (dest NULL or size 0 -> EINVAL)
 * note:  Probes the dest==NULL, size==0 corner. Expected: wcscpy_s and wcscat_s EINVAL with one handler call.
 *        wcsncpy_s(NULL,0,src,0) is believed to be the special "nothing to do" success (0, no handler call);
 *        wcsncat_s(NULL,0,src,0) is believed to be EINVAL with one handler call. These last two are the real probes.
 *        Observed Win10 22H2 ucrtbase: wcsncat_s(NULL,0,src,0) returns 0 with NO handler call (count==0 is a no-op success, like
 *        wcsncpy_s); the other three behave as expected. Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
TC_DECL_PROC(errno_t, wcscat_s, (wchar_t *, size_t, const wchar_t *));
TC_DECL_PROC(errno_t, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
TC_DECL_PROC(errno_t, wcsncat_s, (wchar_t *, size_t, const wchar_t *, size_t));
static wchar_t *volatile vnull = NULL;
static const wchar_t *volatile vsrc = L"a";

TC_CASE(cpy_null_0_null)
{
    int r; if (!tc_p_wcscpy_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; r = tc_p_wcscpy_s(vnull, 0, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cat_null_0_valid)
{
    int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; r = tc_p_wcscat_s(vnull, 0, vsrc);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ncpy_null_0_count0)
{
    int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; r = tc_p_wcsncpy_s(vnull, 0, vsrc, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ncat_null_0_count0)
{
    int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; r = tc_p_wcsncat_s(vnull, 0, vsrc, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);   /* observed */
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcscpy_s); TC_LOAD_PROC(wcscat_s); TC_LOAD_PROC(wcsncpy_s); TC_LOAD_PROC(wcsncat_s);
    TC_RUN(cpy_null_0_null); TC_RUN(cat_null_0_valid); TC_RUN(ncpy_null_0_count0); TC_RUN(ncat_null_0_count0);
    return tc_end();
}
