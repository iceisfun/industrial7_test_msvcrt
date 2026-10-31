/* TC
 * id:    wcs_neg_wcsset_s
 * area:  wstring
 * funcs:
 * procs: _wcsset_s _wcsnset_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _wcsset_s, _strnset_s, _wcsnset_s (error conditions table: NULL, size 0, not NUL-terminated within size -> EINVAL; _wcsnset_s with count < size on an unterminated buffer is split out to wcs_neg_probe_wcsnset_s_unterminated)
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(errno_t, _wcsset_s, (wchar_t *, size_t, wchar_t));
TC_DECL_PROC(errno_t, _wcsnset_s, (wchar_t *, size_t, wchar_t, size_t));
static wchar_t *volatile vnull = NULL;
static volatile size_t v_sz;

TC_CASE(ok_control)
{
    wchar_t b[8] = L"abc"; errno_t e;
    if (!tc_p__wcsset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; v_sz = 8;
    e = tc_p__wcsset_s(b, v_sz, L'z'); CHECK_EQ_I(e, 0); CHECK_WSTR(b, L"zzz"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(set_null)
{
    errno_t e;
    if (!tc_p__wcsset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0; v_sz = 8;
    e = tc_p__wcsset_s(vnull, v_sz, L'z');
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(set_size0)
{
    wchar_t b[4] = L"abc"; errno_t e;
    if (!tc_p__wcsset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0; v_sz = 0;
    e = tc_p__wcsset_s(b, v_sz, L'z');
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(set_unterminated)
{
    wchar_t b[4] = { L'a', L'b', L'c', L'd' }; errno_t e;
    if (!tc_p__wcsset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0; v_sz = 4;
    e = tc_p__wcsset_s(b, v_sz, L'z');
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(nset_null)
{
    errno_t e;
    if (!tc_p__wcsnset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0; v_sz = 8;
    e = tc_p__wcsnset_s(vnull, v_sz, L'z', 2);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(nset_size0)
{
    wchar_t b[4] = L"abc"; errno_t e;
    if (!tc_p__wcsnset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0; v_sz = 0;
    e = tc_p__wcsnset_s(b, v_sz, L'z', 2);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(nset_ok_control)
{
    wchar_t b[8] = L"abcde"; errno_t e;
    if (!tc_p__wcsnset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; v_sz = 8;
    e = tc_p__wcsnset_s(b, v_sz, L'z', 2); CHECK_EQ_I(e, 0); CHECK_WSTR(b, L"zzcde"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_wcsset_s); TC_LOAD_PROC(_wcsnset_s);
    CHECK(tc_p__wcsset_s != NULL); CHECK(tc_p__wcsnset_s != NULL);
    TC_RUN(ok_control); TC_RUN(set_null); TC_RUN(set_size0); TC_RUN(set_unterminated);
    TC_RUN(nset_null); TC_RUN(nset_size0); TC_RUN(nset_ok_control);
    return tc_end();
}
