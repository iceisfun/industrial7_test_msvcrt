/* TC
 * id:    pfo_neg_set_output_format_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _set_output_format, _get_output_format
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_output_format (only _TWO_DIGIT_EXPONENT is defined)
 * note:  PROBE: docs are silent on undefined flag bits. Asserted: the call does not crash and returns the previous value (0) (own cases);
 *        separately that the invalid-parameter handler is not invoked. The no-handler case is a coin flip: the CRT may validate and call the handler. Whether get returns the raw or masked value is not asserted.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _set_output_format, (unsigned int));
TC_DECL_PROC(unsigned int, _get_output_format, (void));
#define _set_output_format tc_p__set_output_format
#define _get_output_format tc_p__get_output_format

TC_CASE(unknown_bit_returns_previous)
{
    CHECK_EQ_U(_set_output_format(0x2), 0);
    _set_output_format(0);
}
TC_CASE(high_bit_returns_previous)
{
    CHECK_EQ_U(_set_output_format(0x80000000u), 0);
    _set_output_format(0);
}
TC_CASE(unknown_bit_no_handler)
{
    tc_iph_count = 0;
    _set_output_format(0x2);
    _set_output_format(0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_output_format); TC_LOAD_PROC(_get_output_format);
    if (!_set_output_format || !_get_output_format) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    tc_iph_install();
    TC_RUN(unknown_bit_returns_previous); TC_RUN(high_bit_returns_previous); TC_RUN(unknown_bit_no_handler);
    return tc_end();
}
