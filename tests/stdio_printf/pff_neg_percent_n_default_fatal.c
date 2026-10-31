/* TC
 * id:    pff_neg_percent_n_default_fatal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf %n; Parameter validation
 * note:  child process
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fatal_child)
{
    DWORD code = 0; int ran = tc_spawn_self("pn", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[64]; volatile int n = 0; volatile double v = 1.5; sprintf(b, "%f%n", v, (int *)&n); return 77; }
    TC_RUN(fatal_child);
    return tc_end();
}
