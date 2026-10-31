/* TC
 * id:    pfo_set_output_format_scprintf_probe
 * area:  stdio_printf
 * funcs:
 * procs: _set_output_format, _scprintf
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_output_format
 * note:  PROBE: _scprintf's returned length follows the _set_output_format setting: "1.500000e+000" is 13 chars, "1.500000e+00" is 12.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _set_output_format, (unsigned int));
TC_DECL_PROC(int, _scprintf, (const char *, ...));
#define _set_output_format tc_p__set_output_format
#define _scprintf tc_p__scprintf

TC_CASE(scprintf_length)
{
    double volatile d = 1.5;
    CHECK_EQ_I(_scprintf("%e", d), 13);
    _set_output_format(1);
    CHECK_EQ_I(_scprintf("%e", d), 12);
    _set_output_format(0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_output_format); TC_LOAD_PROC(_scprintf);
    if (!_set_output_format || !_scprintf) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(scprintf_length);
    return tc_end();
}
