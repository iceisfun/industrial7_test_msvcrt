/* TC
 * id:    scn_neg_child_handler_continue
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, sscanf: if the handler returns, the function returns EOF with errno EINVAL and execution continues
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vs;
TC_CASE(child_continues)
{
    DWORD code = 0; int ran = tc_spawn_self("cont", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        int x = 5, r;
        if (!tc_iph_install()) return 70;
        vs = NULL; tc_iph_count = 0; errno = 0;
        r = sscanf(vs, "%d", &x);
        if (r != EOF) return 71;
        if (tc_iph_count != 1) return 72;
        if (errno != EINVAL) return 73;
        if (x != 5) return 74;
        return 77;
    }
    TC_RUN(child_continues);
    return tc_end();
}
