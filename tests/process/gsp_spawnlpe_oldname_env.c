/* TC
 * id:    gsp_spawnlpe_oldname_env
 * area:  process
 * funcs: _spawnlpe
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnlpe: PATH search plus explicit environment
 * note:  Unprefixed spawnlpe with a full path so PATH search order is not exercised.
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    char b[64]; DWORD n;
    if (tc_strequ(m, "envB")) {
        n = GetEnvironmentVariableA("GSP_B", b, sizeof b);
        return (n == 3 && tc_strequ(b, "two")) ? 0 : 31;
    }
    if (tc_strequ(m, "exit17")) return 17;
    return 90;
}
static char g_self[MAX_PATH], g_sr[MAX_PATH + 16];
TC_CASE(env_visible)
{
    char b[] = "GSP_B=two"; const char *envp[3]; intptr_t r;
    envp[0] = b; envp[1] = g_sr; envp[2] = NULL;
    r = spawnlpe(_P_WAIT, g_self, g_self, "--tc-child=envB", (char *)NULL, envp);
    CHECK_EQ_I(r, 0);
}
TC_CASE(exit_code_17)
{
    char b[] = "GSP_B=two"; const char *envp[3]; intptr_t r;
    envp[0] = b; envp[1] = g_sr; envp[2] = NULL;
    r = spawnlpe(_P_WAIT, g_self, g_self, "--tc-child=exit17", (char *)NULL, envp);
    CHECK_EQ_I(r, 17);
}
int main(void)
{
    const char *c; DWORD n; int i; const char *p = "SystemRoot=";
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    for (i = 0; p[i]; i++) g_sr[i] = p[i];
    n = GetEnvironmentVariableA("SystemRoot", g_sr + i, MAX_PATH);
    if (!n || n >= MAX_PATH) g_sr[i] = 0;
    TC_RUN(env_visible); TC_RUN(exit_code_17);
    return tc_end();
}
