/* TC
 * id:    gsp_spawnv_space_quoting_probe
 * area:  process
 * funcs: _spawnv
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawn ("hi there" is seen as two arguments unless the caller quotes it); UCRT behaviour not documented
 * note:  PROBE: raw child command line for an argv element containing a space. Believed: msvcrt..msvcr120 copy argv verbatim
 *        (per the docs), while ucrtbase wraps an element containing a space in double quotes. If ucrtbase turns out to
 *        be verbatim too, change the TC_UCRT expectation.
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    const char *c = GetCommandLineA(), *k = "--tc-child=";
    int i;
    (void)m;
    for (; *c; c++) { for (i = 0; k[i] && c[i] == k[i]; i++) ; if (!k[i]) break; }
    if (!*c) return 91;
#ifdef TC_UCRT
    return tc_strequ(c, "--tc-child=sp \"a b\"") ? 0 : 92;
#else
    return tc_strequ(c, "--tc-child=sp a b") ? 0 : 92;
#endif
}
static char g_self[MAX_PATH];
TC_CASE(space_arg_raw_cmdline)
{
    const char *av[4]; intptr_t r;
    av[0] = g_self; av[1] = "--tc-child=sp"; av[2] = "a b"; av[3] = NULL;
    r = spawnv(_P_WAIT, g_self, av);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(space_arg_raw_cmdline);
    return tc_end();
}
