/* TC
 * id:    ucr_get_winmain_command_line_child
 * area:  ucrt_only
 * funcs:
 * procs: _get_narrow_winmain_command_line _get_wide_winmain_command_line
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   corecrt_startup.h; WinMain lpCmdLine semantics
 * note:  PROBE: the returned command line has the program name (and following blanks) skipped, so for
 *        '"exe" --tc-child=cmd' it starts with "--tc-child=cmd". Never NULL.
 */
#include "tc.h"

typedef char *(__cdecl *gn_t)(void);
typedef wchar_t *(__cdecl *gw_t)(void);

static int child_main(void)
{
    gn_t gn = (gn_t)tc_crt_proc("_get_narrow_winmain_command_line");
    gw_t gw = (gw_t)tc_crt_proc("_get_wide_winmain_command_line");
    char *n; wchar_t *w;
    if (!gn || !gw) return 70;
    n = gn(); w = gw();
    if (!n) return 71;
    if (!w) return 72;
    if (!tc_strequ(n, "--tc-child=cmd")) return 73;
    if (!tc_wcsequ(w, L"--tc-child=cmd")) return 74;
    return 0;
}
TC_CASE(child) { DWORD code = 99; CHECK(tc_spawn_self("cmd", &code, 20000)); CHECK_EQ_U(code, 0); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(child);
    return tc_end();
}
