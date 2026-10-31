/* TC
 * id:    pfo_neg_set_output_format_absent_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * procs: _get_output_format
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_output_format ("not available in the UCRT"); UCRT release notes: three-digit exponents removed
 * note:  PROBE: ucrtbase.dll does NOT export _set_output_format (the mingw libucrtbase.a also lacks it), and %e always emits at least a
 *        two-digit exponent. The export claim is a negative claim to be confirmed on real Windows.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _get_output_format, (void));

TC_CASE(not_exported)
{
    CHECK(tc_crt_proc("_set_output_format") == NULL);
}
TC_CASE(get_still_zero) { if (!tc_p__get_output_format) TC_SKIP("not exported"); CHECK_EQ_U(tc_p__get_output_format(), 0); }
TC_CASE(two_digit_exponent)
{
    char b[64]; double volatile d = 1.5;
    sprintf(b, "%.1e|%.1E", d, d);
    CHECK_STR(b, "1.5e+00|1.5E+00");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_output_format);
    TC_RUN(not_exported); TC_RUN(get_still_zero); TC_RUN(two_digit_exponent);
    return tc_end();
}
