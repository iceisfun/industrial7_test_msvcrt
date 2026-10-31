/* TC
 * id:    prc_spawnle_env
 * area:  process
 * funcs: _spawnle
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnle: envp becomes the child environment
 * note:  Child (this exe, --tc-child=envchk) verifies TC_PRC_VAR through its exit code.
 */
#include "tc.h"
#include <process.h>

static int child_main(void)
{
    char b[64];
    DWORD n = GetEnvironmentVariableA("TC_PRC_VAR", b, sizeof b);
    return (n == 5 && tc_strequ(b, "hello")) ? 0 : 9;
}
TC_CASE(env_passed)
{
    char exe[MAX_PATH], sr[MAX_PATH], pa[2048], e1[] = "TC_PRC_VAR=hello", e2[MAX_PATH + 16], e3[2100];
    const char *envp[4]; char arg[] = "--tc-child=envchk"; char t[8];
    DWORD n; int i; const char *p1 = "SystemRoot=", *p2 = "PATH=";
    intptr_t r;
    CHECK_EQ_U(GetEnvironmentVariableA("TC_PRC_VAR", t, sizeof t), 0);   /* cannot leak from parent */
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    n = GetEnvironmentVariableA("SystemRoot", sr, MAX_PATH); if (!n || n >= MAX_PATH) sr[0] = 0;
    for (i = 0; p1[i]; i++) e2[i] = p1[i];
    { int j = 0; while (sr[j]) { e2[i + j] = sr[j]; j++; } e2[i + j] = 0; }
    n = GetEnvironmentVariableA("PATH", pa, sizeof pa); if (!n || n >= sizeof pa) pa[0] = 0;
    for (i = 0; p2[i]; i++) e3[i] = p2[i];
    { int j = 0; while (pa[j]) { e3[i + j] = pa[j]; j++; } e3[i + j] = 0; }
    envp[0] = e1; envp[1] = e2; envp[2] = e3; envp[3] = NULL;
    r = _spawnle(_P_WAIT, exe, exe, arg, (char *)NULL, envp);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(env_passed);
    return tc_end();
}
