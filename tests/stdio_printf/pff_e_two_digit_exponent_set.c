/* TC
 * id:    pff_e_two_digit_exponent_set
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_output_format _get_output_format
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_output_format / _get_output_format (VS2005 SP1 .. VS2013)
 * note:  Restores the default format (0) at the end of each case.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _set_output_format, (unsigned int));
TC_DECL_PROC(unsigned int, _get_output_format, (void));
static volatile double v;

TC_CASE(default_zero) { CHECK(tc_p__get_output_format != NULL); if (!tc_p__get_output_format) return; CHECK_EQ_U(tc_p__get_output_format(), 0); }
TC_CASE(set_returns_previous)
{
    unsigned int r;
    if (!tc_p__set_output_format) { CHECK(0); return; }
    r = tc_p__set_output_format(1);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_U(tc_p__get_output_format(), 1);
    r = tc_p__set_output_format(0);
    CHECK_EQ_U(r, 1);
    CHECK_EQ_U(tc_p__get_output_format(), 0);
}
TC_CASE(two_digit_output)
{
    char b[64];
    if (!tc_p__set_output_format) { CHECK(0); return; }
    tc_p__set_output_format(1);
    v = 1.5; sprintf(b, "%e", v); CHECK_STR(b, "1.500000e+00");
    v = 1e-5; sprintf(b, "%g", v); CHECK_STR(b, "1e-05");
    v = 1e100; sprintf(b, "%e", v); CHECK_STR(b, "1.000000e+100");
    tc_p__set_output_format(0);
}
TC_CASE(restore_three_digits)
{
    char b[64];
    if (!tc_p__set_output_format) { CHECK(0); return; }
    tc_p__set_output_format(1); tc_p__set_output_format(0);
    v = 1.5; sprintf(b, "%e", v); CHECK_STR(b, "1.500000e+000");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_output_format); TC_LOAD_PROC(_get_output_format);
    TC_RUN(default_zero); TC_RUN(set_returns_previous); TC_RUN(two_digit_output); TC_RUN(restore_three_digits);
    return tc_end();
}
