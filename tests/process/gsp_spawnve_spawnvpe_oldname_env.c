/* TC
 * id:    gsp_spawnve_spawnvpe_oldname_env
 * area:  process
 * funcs: _spawnve _spawnvpe
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn: envp array becomes the child's environment
 * note:  Unprefixed spawnve/spawnvpe. The cmd.exe case passes SystemRoot/ComSpec explicitly; PATH lookup uses the parent's PATH.
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    char b[64]; DWORD n;
    if (tc_strequ(m, "envC3") || tc_strequ(m, "envC4")) {
        n = GetEnvironmentVariableA("GSP_C", b, sizeof b);
        return (n == 1 && b[0] == (m[4]) && b[1] == 0) ? 0 : 51;
    }
    if (tc_strequ(m, "exit12")) return 12;
    return 90;
}
static char g_self[MAX_PATH], g_sr[MAX_PATH + 16], g_cs[MAX_PATH + 16];
TC_CASE(spawnve_env)
{
    char v[] = "GSP_C=3"; const char *envp[3], *av[3]; intptr_t r;
    envp[0] = v; envp[1] = g_sr; envp[2] = NULL;
    av[0] = g_self; av[1] = "--tc-child=envC3"; av[2] = NULL;
    r = spawnve(_P_WAIT, g_self, av, envp);
    CHECK_EQ_I(r, 0);
}
TC_CASE(spawnvpe_env_full_path)
{
    char v[] = "GSP_C=4"; const char *envp[3], *av[3]; intptr_t r;
    envp[0] = v; envp[1] = g_sr; envp[2] = NULL;
    av[0] = g_self; av[1] = "--tc-child=envC4"; av[2] = NULL;
    r = spawnvpe(_P_WAIT, g_self, av, envp);
    CHECK_EQ_I(r, 0);
}
TC_CASE(spawnve_exit_12)
{
    char v[] = "GSP_C=3"; const char *envp[3], *av[3]; intptr_t r;
    envp[0] = v; envp[1] = g_sr; envp[2] = NULL;
    av[0] = g_self; av[1] = "--tc-child=exit12"; av[2] = NULL;
    r = spawnve(_P_WAIT, g_self, av, envp);
    CHECK_EQ_I(r, 12);
}
TC_CASE(spawnvpe_cmd_exit6)
{
    const char *envp[3], *av[4]; intptr_t r;
    int n = 0;
    envp[n++] = g_sr;
    if (g_cs[8]) envp[n++] = g_cs;
    envp[n] = NULL;
    av[0] = "cmd.exe"; av[1] = "/c"; av[2] = "exit 6"; av[3] = NULL;
    r = spawnvpe(_P_WAIT, "cmd.exe", av, envp);
    CHECK_EQ_I(r, 6);
}
static void setvar(char *dst, const char *name, const char *prefix)
{
    DWORD n; int i;
    for (i = 0; prefix[i]; i++) dst[i] = prefix[i];
    dst[i] = 0;
    n = GetEnvironmentVariableA(name, dst + i, MAX_PATH);
    if (!n || n >= MAX_PATH) dst[i] = 0;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    setvar(g_sr, "SystemRoot", "SystemRoot=");
    setvar(g_cs, "ComSpec", "ComSpec=");
    TC_RUN(spawnve_env); TC_RUN(spawnvpe_env_full_path); TC_RUN(spawnve_exit_12); TC_RUN(spawnvpe_cmd_exit6);
    return tc_end();
}
