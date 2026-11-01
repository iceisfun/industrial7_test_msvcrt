/* TC
 * id:    dir_neg_getcwd_zero_default_fatal
 * area:  dir
 * funcs: _getcwd
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation: default invalid parameter handler terminates the process
 * note:  Child calls _getcwd(buf,0) with no handler. Documented case asserts only abnormal termination (code != 0 and != 77); the exact code 0xC0000409 (fast fail) is probed in its own case.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero;

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("getcwd0", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(probe_exact_code)
{
    DWORD code = 0; int ran = tc_spawn_self("getcwd0", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { char buf[16]; tc_iph_default(); _getcwd(buf, v_zero); return 77; }
    TC_RUN(child_terminates); TC_RUN(probe_exact_code);
    return tc_end();
}
