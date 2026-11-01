/* TC
 * id:    gcv_neg_swab_null_fatal_child
 * area:  conv
 * kind:  negative
 * funcs: _swab
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs "Parameter validation": default handler terminates the process
 * note:  Only termination (exit code neither 0 nor the survival marker 77) is asserted, not the exact status.
 */
#include "tc.h"
#include <stdlib.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(src_null) { run("srcnull"); }
TC_CASE(dst_null) { run("dstnull"); }
TC_CASE(neg_count) { run("neg"); }

static int tc_streq(const char *a, const char *b) { return tc_strequ(a, b); }
int main(void)
{
    const char *c; static char b1[4], b2[4];
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        if (tc_streq(c, "srcnull")) _swab(NULL, b1, 2);
        else if (tc_streq(c, "dstnull")) _swab(b1, NULL, 2);
        else _swab(b1, b2, -1);
        return 77;
    }
    TC_RUN(src_null); TC_RUN(dst_null); TC_RUN(neg_count);
    return tc_end();
}
