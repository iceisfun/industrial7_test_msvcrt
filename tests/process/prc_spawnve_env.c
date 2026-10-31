/* TC
 * id:    prc_spawnve_env
 * area:  process
 * funcs: _spawnve
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnve
 * note:  Child exit code bit0 = TC_PRC_VAR != v2, bit1 = TC_PRC_PARENT visible (must be hidden).
 */
#include "tc.h"
#include <process.h>

static int child_main(void)
{
    char b[64]; int code = 0;
    DWORD n = GetEnvironmentVariableA("TC_PRC_VAR", b, sizeof b);
    if (!(n == 2 && tc_strequ(b, "v2"))) code |= 1;
    if (GetEnvironmentVariableA("TC_PRC_PARENT", b, sizeof b) != 0) code |= 2;
    return code;
}
TC_CASE(env_explicit)
{
    char exe[MAX_PATH], sr[MAX_PATH], pa[2048], e1[] = "TC_PRC_VAR=v2", e2[MAX_PATH + 16], e3[2100];
    const char *envp[4]; const char *av[3]; char arg[] = "--tc-child=envchk2";
    DWORD n; int i; const char *p1 = "SystemRoot=", *p2 = "PATH=";
    intptr_t r;
    SetEnvironmentVariableA("TC_PRC_PARENT", "yes");
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    n = GetEnvironmentVariableA("SystemRoot", sr, MAX_PATH); if (!n || n >= MAX_PATH) sr[0] = 0;
    for (i = 0; p1[i]; i++) e2[i] = p1[i];
    { int j = 0; while (sr[j]) { e2[i + j] = sr[j]; j++; } e2[i + j] = 0; }
    n = GetEnvironmentVariableA("PATH", pa, sizeof pa); if (!n || n >= sizeof pa) pa[0] = 0;
    for (i = 0; p2[i]; i++) e3[i] = p2[i];
    { int j = 0; while (pa[j]) { e3[i + j] = pa[j]; j++; } e3[i + j] = 0; }
    envp[0] = e1; envp[1] = e2; envp[2] = e3; envp[3] = NULL;
    av[0] = exe; av[1] = arg; av[2] = NULL;
    r = _spawnve(_P_WAIT, exe, av, envp);
    CHECK_EQ_I(r, 0);
    SetEnvironmentVariableA("TC_PRC_PARENT", NULL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(env_explicit);
    return tc_end();
}
