/* TC
 * id:    fpe_neg_controlfp_s_null_current_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _controlfp_s (currentControl: "Current control-word bit value"); no explicit NULL statement
 * note:  PROBING: _controlfp_s(NULL, 0, 0) with a returning invalid-parameter handler. Accepted outcomes: (a) returns 0 with no
 *        handler call, (b) returns EINVAL with exactly one handler call. Anything else (crash, other value) fails. Control word must
 *        be unchanged in both. Child process so a crash is contained.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(null_pointer_outcome)
{
    DWORD code = 0; int ran = tc_spawn_self("nullcur", &code, 20000);
    CHECK(ran);
    CHECK(code == 10 || code == 11);   /* 10 = success no handler, 11 = EINVAL + handler */
}
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(_controlfp_s);
    if ((c = tc_child_name()) != NULL) {
        unsigned int before, after; errno_t r;
        if (!tc_p__controlfp_s) return 20;
        if (!tc_iph_install()) return 21;
        before = _controlfp(0, 0);
        tc_iph_count = 0;
        r = tc_p__controlfp_s(NULL, 0, 0);
        after = _controlfp(0, 0);
        if (before != after) return 30;
        if (r == 0 && tc_iph_count == 0) return 10;
        if (r == EINVAL && tc_iph_count == 1) return 11;
        return 12;
    }
    TC_RUN(null_pointer_outcome);
    return tc_end();
}
