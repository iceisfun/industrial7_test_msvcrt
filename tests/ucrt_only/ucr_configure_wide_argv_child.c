/* TC
 * id:    ucr_configure_wide_argv_child
 * area:  ucrt_only
 * funcs:
 * procs: _configure_wide_argv __p___wargv __p___argv
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _configure_wide_argv, __wargv
 * note:  PROBE: calling _configure_wide_argv(1) in a child returns 0 and __wargv[0] is the program name,
 *        equal char by char to narrow argv[0] (both non-NULL). The comparison is skipped if
 *        the path has non-ASCII characters (ANSI vs UTF-16 would legitimately differ).
 */
#include "tc.h"

typedef int      (__cdecl *cfg_t)(int);
typedef wchar_t***(__cdecl *pwargv_t)(void);
typedef char***  (__cdecl *pargv_t)(void);
static cfg_t cfg; static pwargv_t pwargv; static pargv_t pargv;

static int child_main(void)
{
    const wchar_t *w; const char *a; int i;
    if (!cfg || !pwargv || !pargv) return 70;
    if (cfg(1) != 0) return 71;
    if (!*pwargv() || !(*pwargv())[0]) return 72;
    w = (*pwargv())[0]; a = (*pargv())[0];
    if (!a) return 73;
    for (i = 0; w[i] || a[i]; i++) {
        if (w[i] > 127 || (unsigned char)a[i] > 127) return 0;
        if ((int)w[i] != (int)(unsigned char)a[i]) return 74;
    }
    return 0;
}
TC_CASE(child) { DWORD code = 99; CHECK(tc_spawn_self("w", &code, 20000)); CHECK_EQ_U(code, 0); }
int main(void)
{
    tc_begin();
    cfg = (cfg_t)tc_crt_proc("_configure_wide_argv");
    pwargv = (pwargv_t)tc_crt_proc("__p___wargv");
    pargv = (pargv_t)tc_crt_proc("__p___argv");
    if (tc_child_name()) return child_main();
    TC_RUN(child);
    return tc_end();
}
