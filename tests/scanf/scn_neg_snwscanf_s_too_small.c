/* TC
 * id:    scn_neg_snwscanf_s_too_small
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: _snwscanf_s _snscanf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf_s, _snwscanf_s; scanf_s: buffer too small invokes the invalid parameter handler
 * note:  Return value, errno and buffer-emptying are NOT asserted (see scn_neg_secure_too_small_ret_errno_probe).
 *        The count argument limits characters read; the size argument is still checked against what is stored.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef int (__cdecl *sn_t)(const char *, size_t, const char *, ...);
typedef int (__cdecl *snw_t)(const wchar_t *, size_t, const wchar_t *, ...);
static sn_t p_snscanf_s; static snw_t p_snwscanf_s;
#if TC_CRT_VER < 140
#define _snscanf_s p_snscanf_s
#define _snwscanf_s p_snwscanf_s
#endif
static const wchar_t *volatile vw; static const char *volatile vn;
static void fillw(wchar_t *a) { int i; for (i = 0; i < 8; i++) a[i] = L'Z'; }

TC_CASE(wide_s_size3_input_abcdef)
{
    wchar_t a[8]; int n; fillw(a); CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vw = L"abcdef";
    n = _snwscanf_s(vw, 6, L"%s", a, (unsigned)3);
    CHECK(tc_iph_count >= 1); (void)n;
    CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(wide_c_width4_size2)
{
    wchar_t a[8]; int n; fillw(a); CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vw = L"abcdef";
    n = _snwscanf_s(vw, 6, L"%4c", a, (unsigned)2);
    CHECK(tc_iph_count >= 1); (void)n;
    CHECK_EQ_U(a[2], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(wide_set_too_small)
{
    wchar_t a[8]; int n; fillw(a); CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vw = L"abcdef";
    n = _snwscanf_s(vw, 6, L"%[a-z]", a, (unsigned)3);
    CHECK(tc_iph_count >= 1); (void)n;
    CHECK_EQ_U(a[3], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(narrow_count_cut_still_fits_control)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vn = "abcdef";
    n = _snscanf_s(vn, 2, "%s", a, (unsigned)3);
    CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_s_iph_again)
{
    wchar_t a[8]; fillw(a); CHECK(tc_iph_install()); tc_iph_count = 0; vw = L"abcdef";
    (void)_snwscanf_s(vw, 6, L"%s", a, (unsigned)3);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin(); p_snscanf_s = (sn_t)tc_crt_proc("_snscanf_s"); p_snwscanf_s = (snw_t)tc_crt_proc("_snwscanf_s");
    TC_RUN(wide_s_size3_input_abcdef); TC_RUN(wide_c_width4_size2); TC_RUN(wide_set_too_small);
    TC_RUN(narrow_count_cut_still_fits_control); TC_RUN(wide_s_iph_again);
    return tc_end();
}
