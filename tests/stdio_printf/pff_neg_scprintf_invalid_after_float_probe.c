/* TC
 * id:    pff_neg_scprintf_invalid_after_float_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter validation; printf format specification (unknown conversion characters are invalid)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): _scprintf("%f %y", 1.0, 2) does not reject the unknown conversion: it counts it as the literal
 *        character, returning 10 ("1.000000 y"), handler not called, errno untouched. Same for "%y %f" (10). The old expectation (-1/EINVAL/handler)
 *        was wrong for the non-secure UCRT path (only the _s/_p variants validate, see pfo_neg_probe_vsprintf_bad_length_modifiers).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(unknown_conv_after_float)
{
    const char * volatile f = "%f %y"; int r; tc_iph_count = 0; errno = 0;
    r = _scprintf(f, 1.0, 2);
    CHECK_EQ_I(r, 10); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(unknown_conv_before_float)
{
    const char * volatile f = "%y %f"; int r; tc_iph_count = 0; errno = 0;
    r = _scprintf(f, 2, 1.0);
    CHECK_EQ_I(r, 10); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(unknown_conv_after_float); TC_RUN(unknown_conv_before_float);
    return tc_end();
}
