/* TC
 * id:    chr_neg_fgetc_null_fatal
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   MS docs Parameter validation: default handler terminates
 * note:  PROBE: exact termination status of the UCRT default invalid-parameter path for fgetc(NULL), run in a child process (fast-fail 0xC0000409 expected).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullget", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) { FILE *volatile n = NULL; volatile int r = fgetc(n); (void)r; return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
