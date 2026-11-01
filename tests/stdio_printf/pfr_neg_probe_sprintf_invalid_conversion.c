/* TC
 * id:    pfr_neg_probe_sprintf_invalid_conversion
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf type field characters; parameter validation (_set_invalid_parameter_handler)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): the non-secure sprintf does NOT reject an unknown conversion character ('y'): it prints it literally (returns 1, "y"), errno untouched,
 *        no handler call (old guess -1/EINVAL/handler was wrong; the secure sprintf_s does reject it). Narrowed to ucrtbase: msvcr80-120 and msvcrt.dll
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
    CHECK_EQ_I(r, 1);
    CHECK_STR(b, "y");
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(unknown_y); return tc_end();
}
