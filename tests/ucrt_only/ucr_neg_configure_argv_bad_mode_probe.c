/* TC
 * id:    ucr_neg_configure_argv_bad_mode_probe
 * area:  ucrt_only
 * funcs:
 * procs: _configure_narrow_argv _configure_wide_argv
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   corecrt_startup.h _crt_argv_mode (valid: 1, 2)
 * note:  PROBE: modes 3 and -1 (not 0, which is _crt_argv_no_arguments and
 *        believed to be a successful no-op: see ucr_configure_argv_mode0_probe) are rejected with a nonzero return (and EINVAL via the invalid-parameter
 *        handler). Run in a child with the handler installed; exit code 0 if all nonzero, else a case index.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cfg_t)(int);
static int child_main(void)
{
    static const int modes[2] = { 3, -1 }; int k, bad = 0, w;
    cfg_t f[2]; f[0] = (cfg_t)tc_crt_proc("_configure_narrow_argv"); f[1] = (cfg_t)tc_crt_proc("_configure_wide_argv");
    if (!f[0] || !f[1] || !tc_iph_install()) return 70;
    for (w = 0; w < 2; w++) for (k = 0; k < 2; k++) {
        int r; errno = 0; tc_iph_count = 0;
        r = f[w](modes[k]);
        if (r == 0 || errno != EINVAL || tc_iph_count != 1) bad |= 1 << (w * 2 + k);
    }
    return bad;
}
TC_CASE(bad_modes) { DWORD code = 99; CHECK(tc_spawn_self("bad", &code, 20000)); CHECK_EQ_U(code, 0); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(bad_modes);
    return tc_end();
}
