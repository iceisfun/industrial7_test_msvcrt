/* TC
 * id:    wcs_neg_wcscpy_s_small
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s, _mbscpy_s (error condition table)
 * note:  Table: dest NULL or size 0 -> EINVAL (dest untouched); src NULL -> EINVAL, dest[0]=0; too small -> ERANGE, dest[0]=0.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcscpy_s, (wchar_t *, size_t, const wchar_t *));

TC_CASE(too_small)
{
    wchar_t d[8] = { L'X', L'X', L'X', L'X', L'X', L'X', L'X', 0 };
    const wchar_t * volatile s = L"abcd"; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p_wcscpy_s(d, 4, s);   /* needs 5 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(exact_fit)
{
    wchar_t d[8] = { L'X', L'X', L'X', L'X', L'X', L'X', L'X', 0 };
    const wchar_t * volatile s = L"abcd"; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p_wcscpy_s(d, 5, s);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_WSTR(d, L"abcd");
}
TC_CASE(size_zero)
{
    wchar_t d[4] = { L'X', 0 };
    const wchar_t * volatile s = L"a"; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p_wcscpy_s(d, 0, s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], L'X');
}
TC_CASE(dest_null)
{
    const wchar_t * volatile s = L"a"; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p_wcscpy_s(NULL, 4, s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(src_null)
{
    wchar_t d[4] = { L'X', L'X', L'X', 0 }; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p_wcscpy_s(d, 4, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcscpy_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(too_small); TC_RUN(exact_fit); TC_RUN(size_zero); TC_RUN(dest_null); TC_RUN(src_null);
    return tc_end();
}
