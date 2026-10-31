/* TC
 * id:    nfx_neg_abort_sig_ign
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort signal

 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs abort
 * note:  PROBE: abort() with SIGABRT set to SIG_IGN; expected to still terminate with code 3.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

TC_CASE(ignored)
{
    DWORD code = 0; int ran = tc_spawn_self("ign", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) { signal(SIGABRT, SIG_IGN); abort(); return 77; }
    TC_RUN(ignored);
    return tc_end();
}
