/* TC
 * id:    pfr_neg_probe_sprintf_invalid_conversion
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf type field characters; parameter validation (_set_invalid_parameter_handler)
 * note:  PROBE: an unknown conversion character ('y') is rejected as an invalid parameter: returns -1, errno EINVAL,
 *        handler called once (rather than printing the text literally). Narrowed to ucrtbase: msvcr80-120 and msvcrt.dll
 *        may treat unknown type chars after % differently (literal output vs invalid parameter); unverified there.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(unknown_y)
{
    char b[32]; const char * volatile f = "%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(unknown_y); return tc_end();
}
