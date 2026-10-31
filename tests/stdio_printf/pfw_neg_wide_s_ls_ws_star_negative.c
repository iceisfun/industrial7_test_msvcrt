/* TC
 * id:    pfw_neg_wide_s_ls_ws_star_negative
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.1p5-6 (negative field width from * is a '-' flag; negative precision is taken as omitted); MS docs printf width/precision
 * note:  Negative * arguments are not errors: no invalid-parameter handler call is expected.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile ab = L"ab";
static const wchar_t *volatile abc = L"abc";
static volatile int m5 = -5, m3 = -3, m1 = -1, five = 5;

TC_CASE(neg_width_ls) { wchar_t b[20]; int n; tc_iph_count = 0; n = _snwprintf(b, 20, L"%*ls|", m5, ab); CHECK_EQ_I(n, 6); CHECK_WSTR(b, L"ab   |"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(neg_prec_ls) { wchar_t b[20]; int n; tc_iph_count = 0; n = _snwprintf(b, 20, L"%.*ls|", m1, abc); CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"abc|"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(neg_width_c) { wchar_t b[20]; int n; tc_iph_count = 0; n = _snwprintf(b, 20, L"%*c|", m3, (wint_t)L'x'); CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"x  |"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(neg_prec_d) { wchar_t b[20]; int n; tc_iph_count = 0; n = _snwprintf(b, 20, L"%.*d", m1, five); CHECK_EQ_I(n, 1); CHECK_WSTR(b, L"5"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(neg_width_ls); TC_RUN(neg_prec_ls); TC_RUN(neg_width_c); TC_RUN(neg_prec_d);
    return tc_end();
}
