/* TC
 * id:    pfq_sprintf_p_flags_width_prec
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  The '.0d of 0' and star cases live in pfq_probe_sprintf_p_edge_cases.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$05d";
static const char * volatile f2 = "%1$+d %2$ d";
static const char * volatile f3 = "%1$#x %1$#o";
static const char * volatile f4 = "%1$.3s";
static const char * volatile f5 = "%2$-6d|%1$6d|";
static const char * volatile f6 = "%1$-8.3f|%1$+.1f";

TC_CASE(zero_pad) { char b[32]; _sprintf_p(b, sizeof b, f1, 42); CHECK_STR(b, "00042"); }
TC_CASE(plus_space) { char b[32]; _sprintf_p(b, sizeof b, f2, 5, 6); CHECK_STR(b, "+5  6"); }
TC_CASE(alt_forms) { char b[32]; _sprintf_p(b, sizeof b, f3, 255); CHECK_STR(b, "0xff 0377"); }
TC_CASE(str_prec) { char b[32]; _sprintf_p(b, sizeof b, f4, "abcdef"); CHECK_STR(b, "abc"); }
TC_CASE(left_right) { char b[32]; int n = _sprintf_p(b, sizeof b, f5, 1, 2); CHECK_STR(b, "2     |     1|"); CHECK_EQ_I(n, 14); }
TC_CASE(float_flags) { char b[32]; double v = 2.5; _sprintf_p(b, sizeof b, f6, v); CHECK_STR(b, "2.500   |+2.5"); }

int main(void) { tc_begin(); TC_RUN(zero_pad); TC_RUN(plus_space); TC_RUN(alt_forms); TC_RUN(str_prec); TC_RUN(left_right); TC_RUN(float_flags); return tc_end(); }
