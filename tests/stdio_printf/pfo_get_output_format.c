/* TC
 * id:    pfo_get_output_format
 * area:  stdio_printf
 * funcs:
 * procs: _get_output_format
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _get_output_format, _set_output_format (the _TWO_DIGIT_EXPONENT mechanism does not exist in the UCRT)
 * note:  class D. OBSERVED Win10 22H2 19045 ucrtbase (GetProcAddress in the reference VM, run twice): _get_output_format is NOT exported (nor is _set_output_format). The old premise
 *        "ucrtbase exports _get_output_format" was wrong, so the export case now asserts absence; the value cases stay and skip if a future build exports it.
 *        Build-dependent: export tables can change between UCRT builds.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _get_output_format, (void));
#define _get_output_format tc_p__get_output_format

TC_CASE(exported) { CHECK(_get_output_format == NULL); }
TC_CASE(returns_zero) {if (!_get_output_format) TC_SKIP("not exported");  CHECK_EQ_U(_get_output_format(), 0); }
TC_CASE(repeatable)
{
    int i;
    if (!_get_output_format) TC_SKIP("not exported");
    for (i = 0; i < 3; i++) CHECK_EQ_U(_get_output_format(), 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_output_format);
    TC_RUN(exported); TC_RUN(returns_zero); TC_RUN(repeatable);
    return tc_end();
}
