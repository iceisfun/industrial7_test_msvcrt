/* TC
 * id:    ext_neg_onexit_null_func
 * area:  exit_signal
 * funcs: 
 * procs: _onexit
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _onexit (returns NULL on failure)
 * note:  Probes that _onexit(NULL) does not crash a later exit(). The return value is NULL whether rejected or
 *        (accepted and returning its argument), so case rejected cannot discriminate and only checks the child chose 10. Either way, that exit() afterwards terminates
 *        normally with the requested code (no crash on a NULL table entry). Case 'survives' accepts 10 or 11;
 *        case 'rejected' probes the specific value 10.
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *onexit_fn)(void);
typedef onexit_fn (__cdecl *onexit_pf)(onexit_fn);

static onexit_pf f;
static DWORD run(void)
{
    DWORD code = 0; tc_spawn_self("on", &code, 20000); return code;
}
TC_CASE(survives) { if (!f) TC_SKIP("_onexit not exported");  DWORD c = run(); CHECK(c == 10 || c == 11); }
TC_CASE(rejected) { if (!f) TC_SKIP("_onexit not exported");  CHECK_EQ_U(run(), 10); }

int main(void)
{
    tc_begin();
    f = (onexit_pf)tc_crt_proc("_onexit");
    if (tc_child_name()) {
        onexit_fn r;
        if (!f) return 12;
        r = f((onexit_fn)(volatile void *)NULL);
        exit(r == NULL ? 10 : 11);
        return 77;
    }
    TC_RUN(survives); TC_RUN(rejected);
    return tc_end();
}
