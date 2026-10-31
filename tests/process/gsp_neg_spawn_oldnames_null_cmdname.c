/* TC
 * id:    gsp_neg_spawn_oldnames_null_cmdname
 * area:  process
 * funcs: _spawnl _spawnle _spawnlp _spawnlpe _spawnv _spawnve _spawnvp _spawnvpe
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs "Parameter validation" (EINVAL + invalid parameter handler); NULL cmdname not spelled out per function
 * note:  Unprefixed names. msvcrt.dll excluded (no invalid-parameter handler API). The handler installed by tc_iph_install returns.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static const char *volatile g_nul = NULL;
static const char *volatile g_a0 = "x";
static char *g_envp[] = { (char *)"GSP_X=1", NULL };
static char *g_argv[] = { (char *)"x", NULL };

static void pre(void) { if (!tc_iph_install()) return; tc_iph_count = 0; errno = 0; }
static void post(intptr_t r)
{
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(spawnl)   { pre(); post(spawnl(_P_WAIT, g_nul, g_a0, (char *)NULL)); }
TC_CASE(spawnle)  { pre(); post(spawnle(_P_WAIT, g_nul, g_a0, (char *)NULL, g_envp)); }
TC_CASE(spawnlp)  { pre(); post(spawnlp(_P_WAIT, g_nul, g_a0, (char *)NULL)); }
TC_CASE(spawnlpe) { pre(); post(spawnlpe(_P_WAIT, g_nul, g_a0, (char *)NULL, g_envp)); }
TC_CASE(spawnv)   { pre(); post(spawnv(_P_WAIT, g_nul, g_argv)); }
TC_CASE(spawnve)  { pre(); post(spawnve(_P_WAIT, g_nul, g_argv, g_envp)); }
TC_CASE(spawnvp)  { pre(); post(spawnvp(_P_WAIT, g_nul, g_argv)); }
TC_CASE(spawnvpe) { pre(); post(spawnvpe(_P_WAIT, g_nul, g_argv, g_envp)); }
int main(void)
{
    tc_begin();
    TC_RUN(spawnl); TC_RUN(spawnle); TC_RUN(spawnlp); TC_RUN(spawnlpe);
    TC_RUN(spawnv); TC_RUN(spawnve); TC_RUN(spawnvp); TC_RUN(spawnvpe);
    return tc_end();
}
