/* TC
 * id:    ucr_configure_argv_mode0_probe
 * area:  ucrt_only
 * funcs:
 * procs: _configure_narrow_argv _configure_wide_argv
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   corecrt_startup.h _crt_argv_mode (_crt_argv_no_arguments = 0)
 * note:  PROBE: mode 0 (_crt_argv_no_arguments) is accepted as a no-op and returns 0 without invoking the
 *        invalid-parameter handler. Run in a child.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cfg_t)(int);
static int child_main(void)
{
    cfg_t n = (cfg_t)tc_crt_proc("_configure_narrow_argv"), w = (cfg_t)tc_crt_proc("_configure_wide_argv");
    if (!n || !w || !tc_iph_install()) return 70;
    tc_iph_count = 0;
    if (n(0) != 0) return 71;
    if (w(0) != 0) return 72;
    if (tc_iph_count != 0) return 73;
    return 0;
}
TC_CASE(mode0) { DWORD code = 99; CHECK(tc_spawn_self("m0", &code, 20000)); CHECK_EQ_U(code, 0); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(mode0);
    return tc_end();
}
