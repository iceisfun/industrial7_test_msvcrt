/* TC
 * id:    gsp_neg_popen_null_fatal_child
 * area:  process
 * funcs: _popen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation / _set_invalid_parameter_handler: with no handler the default terminates the application
 * note:  Unprefixed popen(NULL,"r"). Child "fatal" has no handler installed and must not survive to return 77. Exact status
 *        (0xC0000409 on ucrt) is not asserted. Child "ctrl" installs tc_iph and must see NULL + one handler call -> exit 77.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile g_nul = NULL;

static int child_main(const char *m)
{
    FILE *volatile f;
    if (tc_strequ(m, "fatal")) { tc_iph_default(); f = popen(g_nul, "r"); (void)f; return 77; }
    if (tc_strequ(m, "ctrl")) {
        if (!tc_iph_install()) return 79;
        f = popen(g_nul, "r");
        return (f == NULL && tc_iph_count == 1) ? 77 : 78;
    }
    return 90;
}
TC_CASE(default_handler_terminates)
{
    DWORD code = 77; int ran = tc_spawn_self("fatal", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}
TC_CASE(control_with_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("ctrl", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(default_handler_terminates); TC_RUN(control_with_handler);
    return tc_end();
}
