/* TC
 * id:    gsf_neg_clearerr_s_default_handler_fatal
 * area:  stdio_file
 * funcs: clearerr_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  Fatal path runs in a child process. The generic termination check is documented; the exact status code check (fast-fail 0xC0000409) is a probe and lives in its own case/test.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static FILE *volatile nullfp;

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullclr", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if ((c = tc_child_name()) != NULL) { clearerr_s(nullfp); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
