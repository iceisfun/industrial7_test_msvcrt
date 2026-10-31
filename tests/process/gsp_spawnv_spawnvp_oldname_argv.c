/* TC
 * id:    gsp_spawnv_spawnvp_oldname_argv
 * area:  process
 * funcs: _spawnv _spawnvp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn: argv array form
 * note:  Unprefixed spawnv/spawnvp. Whether embedded spaces get quoted is NOT asserted here (see gsp_spawnv_space_quoting_probe).
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    const char *c = GetCommandLineA(), *k = "--tc-child=";
    int i;
    for (; *c; c++) { for (i = 0; k[i] && c[i] == k[i]; i++) ; if (!k[i]) break; }
    if (!*c) return 91;
    if (tc_strequ(m, "args")) return tc_strequ(c, "--tc-child=args alpha beta") ? 0 : 92;
    if (tc_strequ(m, "single")) return tc_strequ(c, "--tc-child=single") ? 0 : 93;
    return 90;
}
static char g_self[MAX_PATH];

TC_CASE(spawnv_tokens)
{
    const char *av[5]; intptr_t r;
    av[0] = g_self; av[1] = "--tc-child=args"; av[2] = "alpha"; av[3] = "beta"; av[4] = NULL;
    r = spawnv(_P_WAIT, g_self, av);
    CHECK_EQ_I(r, 0);
}
TC_CASE(spawnvp_cmd_exit4)
{
    const char *av[4]; intptr_t r;
    av[0] = "cmd.exe"; av[1] = "/c"; av[2] = "exit 4"; av[3] = NULL;
    r = spawnvp(_P_WAIT, "cmd.exe", av);
    CHECK_EQ_I(r, 4);
}
TC_CASE(spawnv_single_element)
{
    /* argv holds only argv[0]: the only token is the program itself plus our marker via a second element */
    const char *av[3]; intptr_t r;
    av[0] = g_self; av[1] = "--tc-child=single"; av[2] = NULL;
    r = spawnv(_P_WAIT, g_self, av);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(spawnv_tokens); TC_RUN(spawnvp_cmd_exit4); TC_RUN(spawnv_single_element);
    return tc_end();
}
