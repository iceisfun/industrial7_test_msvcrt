/* TC
 * id:    gsp_spawnl_oldname_wait
 * area:  process
 * funcs: _spawnl
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions (POSIX name spawnl is a deprecated alias of _spawnl)
 * note:  Calls the unprefixed source-level name spawnl. The mingw headers/oldnames thunk binds it to the DLL's _spawnl,
 *        hence funcs lists _spawnl. Child = this exe with --tc-child=<mode>; its exit code is the observable.
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    const char *c = GetCommandLineA(), *k = "--tc-child=";
    int i;
    if (tc_strequ(m, "exit5")) return 5;
    if (tc_strequ(m, "exit0")) return 0;
    for (; *c; c++) { for (i = 0; k[i] && c[i] == k[i]; i++) ; if (!k[i]) break; }
    if (!*c) return 91;
    if (tc_strequ(m, "args")) return tc_strequ(c, "--tc-child=args alpha beta") ? 0 : 92;
    if (tc_strequ(m, "arg0")) {
        const char *g = GetCommandLineA(), *want = "GSPARG0 ";
        for (i = 0; want[i]; i++) if (g[i] != want[i]) return 93;
        return 0;
    }
    return 90;
}
static char g_self[MAX_PATH];

TC_CASE(exit_code_5)
{
    intptr_t r = spawnl(_P_WAIT, g_self, g_self, "--tc-child=exit5", (char *)NULL);
    CHECK_EQ_I(r, 5);
}
TC_CASE(exit_code_0)
{
    intptr_t r = spawnl(_P_WAIT, g_self, g_self, "--tc-child=exit0", (char *)NULL);
    CHECK_EQ_I(r, 0);
}
TC_CASE(args_passed)
{
    intptr_t r = spawnl(_P_WAIT, g_self, g_self, "--tc-child=args", "alpha", "beta", (char *)NULL);
    CHECK_EQ_I(r, 0);
}
TC_CASE(arg0_is_first_token)
{
    /* arg0 becomes the first token of the child's command line, independent of the file name */
    intptr_t r = spawnl(_P_WAIT, g_self, "GSPARG0", "--tc-child=arg0", (char *)NULL);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(exit_code_5); TC_RUN(exit_code_0); TC_RUN(args_passed); TC_RUN(arg0_is_first_token);
    return tc_end();
}
