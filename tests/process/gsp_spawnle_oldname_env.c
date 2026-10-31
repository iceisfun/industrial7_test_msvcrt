/* TC
 * id:    gsp_spawnle_oldname_env
 * area:  process
 * funcs: _spawnle
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn: envp array sets the environment of the new process
 * note:  Unprefixed spawnle (bound by the oldnames thunk to _spawnle). Child uses kernel32 only.
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    char b[64];
    DWORD n;
    if (tc_strequ(m, "envA")) {
        n = GetEnvironmentVariableA("GSP_A", b, sizeof b);
        if (!(n == 3 && tc_strequ(b, "one"))) return 11;
        if (GetEnvironmentVariableA("GSP_PARENT", b, sizeof b) != 0) return 12;
        return 0;
    }
    if (tc_strequ(m, "env2")) {
        n = GetEnvironmentVariableA("GSP_A", b, sizeof b);
        if (!(n == 3 && tc_strequ(b, "one"))) return 21;
        n = GetEnvironmentVariableA("GSP_B", b, sizeof b);
        if (!(n == 3 && tc_strequ(b, "two"))) return 22;
        return 0;
    }
    return 90;
}
static char g_self[MAX_PATH], g_sr[MAX_PATH + 16];
static void init(void)
{
    DWORD n; int i; const char *p = "SystemRoot=";
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    for (i = 0; p[i]; i++) g_sr[i] = p[i];
    n = GetEnvironmentVariableA("SystemRoot", g_sr + i, MAX_PATH);
    if (!n || n >= MAX_PATH) g_sr[i] = 0;
}
TC_CASE(one_var_parent_var_absent)
{
    char a[] = "GSP_A=one"; const char *envp[3]; intptr_t r;
    envp[0] = a; envp[1] = g_sr; envp[2] = NULL;
    SetEnvironmentVariableA("GSP_PARENT", "yes");
    r = spawnle(_P_WAIT, g_self, g_self, "--tc-child=envA", (char *)NULL, envp);
    SetEnvironmentVariableA("GSP_PARENT", NULL);
    CHECK_EQ_I(r, 0);
}
TC_CASE(two_vars_visible)
{
    char a[] = "GSP_A=one", b[] = "GSP_B=two"; const char *envp[4]; intptr_t r;
    envp[0] = a; envp[1] = b; envp[2] = g_sr; envp[3] = NULL;
    r = spawnle(_P_WAIT, g_self, g_self, "--tc-child=env2", (char *)NULL, envp);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    init();
    TC_RUN(one_var_parent_var_absent); TC_RUN(two_vars_visible);
    return tc_end();
}
