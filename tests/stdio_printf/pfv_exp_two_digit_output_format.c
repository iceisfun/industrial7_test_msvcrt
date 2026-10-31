/* TC
 * id:    pfv_exp_two_digit_output_format
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_output_format _get_output_format
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _set_output_format, _get_output_format
 * note:  _set_output_format is not available in ucrtbase; both functions resolved at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_DECL_PROC(unsigned, _set_output_format, (unsigned));
TC_DECL_PROC(unsigned, _get_output_format, (void));
static volatile double v1e5 = 1e5, v1e100 = 1e100;
TC_CASE(default_returns_0) {
    char b[64];
    if (!tc_p__set_output_format) TC_SKIP("not exported");
    sprintf(b, "%e", v1e5); CHECK_STR(b, "1.000000e+005");
    if (tc_p__get_output_format) CHECK_EQ_U(tc_p__get_output_format(), 0);
}
TC_CASE(switch_two_digit) {
    char b[64]; unsigned prev;
    if (!tc_p__set_output_format) TC_SKIP("not exported");
    prev = tc_p__set_output_format(1);
    CHECK_EQ_U(prev, 0);
    sprintf(b, "%e", v1e5); CHECK_STR(b, "1.000000e+05");
    if (tc_p__get_output_format) CHECK_EQ_U(tc_p__get_output_format(), 1);
    tc_p__set_output_format(0);
}
TC_CASE(big_exp_stays_3) {
    char b[64]; 
    if (!tc_p__set_output_format) TC_SKIP("not exported");
    tc_p__set_output_format(1);
    sprintf(b, "%e", v1e100); CHECK_STR(b, "1.000000e+100");
    tc_p__set_output_format(0);
}
TC_CASE(restore) {
    char b[64]; unsigned prev;
    if (!tc_p__set_output_format) TC_SKIP("not exported");
    tc_p__set_output_format(1);
    prev = tc_p__set_output_format(0);
    CHECK_EQ_U(prev, 1);
    sprintf(b, "%e", v1e5); CHECK_STR(b, "1.000000e+005");
}
int main(void) { tc_begin(); TC_LOAD_PROC(_set_output_format); TC_LOAD_PROC(_get_output_format);
  TC_RUN(default_returns_0); TC_RUN(switch_two_digit); TC_RUN(big_exp_stays_3); TC_RUN(restore); return tc_end(); }
