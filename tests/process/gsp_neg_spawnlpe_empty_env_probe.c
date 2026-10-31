/* TC
 * id:    gsp_neg_spawnlpe_empty_env_probe
 * area:  process
 * funcs: _spawnlpe
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn (envp); empty array behaviour is not spelled out
 * note:  PROBE: an envp consisting only of the terminating NULL means "empty environment" (NOT "inherit"), so a variable
 *        set in the parent (GSP_B) must be absent in the child, and the spawn itself still succeeds (child exit 0).
 */
#include "tc.h"
#include <process.h>

static int child_main(const char *m)
{
    char b[8];
    (void)m;
    return GetEnvironmentVariableA("GSP_B", b, sizeof b) == 0 ? 0 : 41;
}
TC_CASE(parent_var_absent)
{
    char self[MAX_PATH]; const char *envp[1]; intptr_t r;
    GetModuleFileNameA(NULL, self, MAX_PATH);
    envp[0] = NULL;
    SetEnvironmentVariableA("GSP_B", "parent");
    r = spawnlpe(_P_WAIT, self, self, "--tc-child=noB", (char *)NULL, envp);
    SetEnvironmentVariableA("GSP_B", NULL);
    CHECK_EQ_I(r, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(parent_var_absent);
    return tc_end();
}
