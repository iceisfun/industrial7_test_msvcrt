/* TC
 * id:    gsp_neg_spawn_oldnames_enoent
 * area:  process
 * funcs: _spawnl _spawnle _spawnlp _spawnlpe _spawnv _spawnve _spawnvp _spawnvpe
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions: Return Value (-1 on error) and errno ENOENT (file or path not found)
 * note:  Unprefixed source-level names (thunks to the underscore exports). A nonexistent program is a runtime failure,
 *        not an invalid parameter: the invalid-parameter handler must NOT run (checked only on CRTs that have the API, >=80).
 *        _P_NOWAIT must also return -1, not a process handle.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static const char *volatile g_prog = "gsp_no_such.exe";
static char *g_envp[] = { (char *)"GSP_X=1", NULL };
static char *g_argv[] = { (char *)"gsp_no_such.exe", NULL };
static int g_have_iph;

static void pre(void) { g_have_iph = tc_iph_install(); tc_iph_count = 0; errno = 0; }
static void post(intptr_t r)
{
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, ENOENT);
    if (g_have_iph) CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(spawnl)   { pre(); post(spawnl(_P_WAIT, g_prog, g_prog, (char *)NULL)); }
TC_CASE(spawnle)  { pre(); post(spawnle(_P_WAIT, g_prog, g_prog, (char *)NULL, g_envp)); }
TC_CASE(spawnlp)  { pre(); post(spawnlp(_P_WAIT, g_prog, g_prog, (char *)NULL)); }
TC_CASE(spawnlpe) { pre(); post(spawnlpe(_P_WAIT, g_prog, g_prog, (char *)NULL, g_envp)); }
TC_CASE(spawnv)   { pre(); post(spawnv(_P_WAIT, g_prog, g_argv)); }
TC_CASE(spawnve)  { pre(); post(spawnve(_P_WAIT, g_prog, g_argv, g_envp)); }
TC_CASE(spawnvp_vpe)
{
    pre(); post(spawnvp(_P_WAIT, g_prog, g_argv));
    pre(); post(spawnvpe(_P_WAIT, g_prog, g_argv, g_envp));
}
TC_CASE(nowait_no_handle)
{
    pre(); post(spawnl(_P_NOWAIT, g_prog, g_prog, (char *)NULL));
    pre(); post(spawnvp(_P_NOWAIT, g_prog, g_argv));
}
int main(void)
{
    tc_begin();
    TC_RUN(spawnl); TC_RUN(spawnle); TC_RUN(spawnlp); TC_RUN(spawnlpe);
    TC_RUN(spawnv); TC_RUN(spawnve); TC_RUN(spawnvp_vpe);
    TC_RUN(nowait_no_handle);
    return tc_end();
}
