/* TC
 * id:    nfx_neg_onexit_null_exit_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs:  exit
 * procs: _onexit
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _onexit
 * note:  Probes: registering a NULL handler (whatever _onexit returns) never makes a later exit(5) crash; status stays 5. Default handler would be fatal if NULL validated, so a returning iph is installed in the child.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


TC_CASE(null_then_exit)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("go", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 5);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if (tc_child_name() != NULL) {
        volatile _onexit_t f = NULL;
        tc_iph_install();
        tc_p__onexit(f);
        exit(5);
    }
    TC_RUN(null_then_exit);
    return tc_end();
}
