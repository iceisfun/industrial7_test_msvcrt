/* TC
 * id:    pfq_sprintf_p_types
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  %hd truncation to short is standard C; %p only compared against non-positional %p. hh is tested separately (probe).
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_mix = "%5$lld|%4$u|%3$x|%2$s|%1$c";
static const char * volatile f_dbl = "%3$g,%2$e,%1$f";
static const char * volatile f_i64 = "%1$I64d %2$lld";
static const char * volatile f_h = "%1$hd";
static const char * volatile f_pp = "%1$p";
static const char * volatile f_pn = "%p";
static const char * volatile f_ls = "[%1$ls]";

TC_CASE(shuffled_ints)
{
    char b[96]; _sprintf_p(b, sizeof b, f_mix, 'Q', "str", 255, 4000000000u, -5000000000LL);
    CHECK_STR(b, "-5000000000|4000000000|ff|str|Q");
}
TC_CASE(doubles_reverse)
{
    char b[96]; double a = 1.5, c = 2.5, d = 0.125;
    _sprintf_p(b, sizeof b, f_dbl, a, c, d);
#ifdef _UCRT
    CHECK_STR(b, "0.125,2.500000e+00,1.500000");
#else
    CHECK_STR(b, "0.125,2.500000e+000,1.500000"); /* pre-VS2015 CRTs print a 3-digit exponent by default */
#endif
}
TC_CASE(int64) { char b[64]; _sprintf_p(b, sizeof b, f_i64, 0x123456789LL, 0x123456789LL); CHECK_STR(b, "4886718345 4886718345"); }
TC_CASE(short_trunc) { char b[32]; _sprintf_p(b, sizeof b, f_h, 70000); CHECK_STR(b, "4464"); }
TC_CASE(pointer_same)
{
    char a[64], c[64]; void *p = (void *)&a;
    _sprintf_p(a, sizeof a, f_pp, p); _sprintf_p(c, sizeof c, f_pn, p);
    CHECK(a[0] != 0);
    CHECK_STR(a, c);
}
TC_CASE(wide_string_narrow_out)
{
    char b[32]; static const wchar_t w[] = L"wide"; _sprintf_p(b, sizeof b, f_ls, w); CHECK_STR(b, "[wide]");
}

int main(void) { tc_begin(); TC_RUN(shuffled_ints); TC_RUN(doubles_reverse); TC_RUN(int64); TC_RUN(short_trunc); TC_RUN(pointer_same); TC_RUN(wide_string_narrow_out); return tc_end(); }
