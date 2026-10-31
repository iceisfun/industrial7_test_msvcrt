/* TC
 * id:    scn_neg_snscanf_s_too_small
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf_s _snwscanf_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snscanf_s, _snwscanf_s: destination size too small invokes the invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vin; static const wchar_t *volatile vwin;
TC_CASE(narrow)
{
    char a[8]; int i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefgh";
    (void)_snscanf_s((const char *)vin, 8, "%s", a, (unsigned)4);
    CHECK(tc_iph_count >= 1); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
TC_CASE(wide)
{
    wchar_t a[8]; int i; for (i = 0; i < 8; i++) a[i] = L'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vwin = L"abcdefgh";
    (void)_snwscanf_s((const wchar_t *)vwin, 8, L"%s", a, (unsigned)4);
    CHECK(tc_iph_count >= 1); CHECK_EQ_U(a[4], L'Z'); CHECK_EQ_U(a[7], L'Z');
}
TC_CASE(count_smaller_control)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefgh";
    n = _snscanf_s((const char *)vin, 3, "%s", a, (unsigned)4);
    CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); TC_RUN(count_smaller_control); return tc_end(); }
