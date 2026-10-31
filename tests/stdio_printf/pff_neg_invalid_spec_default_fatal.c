/* TC
 * id:    pff_neg_invalid_spec_default_fatal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf; Parameter validation
 * note:  PROBE (see pff_neg_invalid_conversion_after_float): "%f %y" is fatal under the default handler. child process
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fatal_child)
{
    DWORD code = 0; int ran = tc_spawn_self("badspec", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[64]; volatile double v = 1.5; const char *volatile f = "%f %y"; sprintf(b, f, v, 1); return 77; }
    TC_RUN(fatal_child);
    return tc_end();
}
