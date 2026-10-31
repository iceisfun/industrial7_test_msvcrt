/* TC
 * id:    gsp_neg_spawnv_oldnames_null_argv
 * area:  process
 * funcs: _spawnv _spawnve _spawnvp _spawnvpe
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _spawn functions (argv must be a non-NULL array whose first element is the program name); Parameter validation
 * note:  Recursion guard: named mutex Local\tc_gsp_norec_argv_<dll> held by the real run makes an accidentally spawned copy exit 0 at once.
 *        The program path is this exe, so if validation were absent a real child would start (with a bogus command line).
 *        Empty-array case (argv[0]==NULL) is its own case so one wrong guess does not mask the NULL-array cases.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_self[MAX_PATH];
static char *const *volatile g_nullv = NULL;
static char *g_envp[] = { (char *)"GSP_X=1", NULL };
static char *g_empty[] = { NULL };

static void pre(void) { if (!tc_iph_install()) return; tc_iph_count = 0; errno = 0; }
static void post(intptr_t r)
{
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(spawnv_null)   { pre(); post(spawnv(_P_WAIT, g_self, g_nullv)); }
TC_CASE(spawnve_null)  { pre(); post(spawnve(_P_WAIT, g_self, g_nullv, g_envp)); }
TC_CASE(spawnvp_null)  { pre(); post(spawnvp(_P_WAIT, g_self, g_nullv)); }
TC_CASE(spawnvpe_null) { pre(); post(spawnvpe(_P_WAIT, g_self, g_nullv, g_envp)); }
TC_CASE(spawnv_empty_array) { pre(); post(spawnv(_P_WAIT, g_self, g_empty)); }
int main(void)
{
    tc_begin();
    { HANDLE gh = CreateMutexA(NULL, FALSE, "Local\\tc_gsp_norec_argv_" TC_CRT_DLL);
      if (gh && GetLastError() == ERROR_ALREADY_EXISTS) return 0;   /* accidentally spawned copy of this exe: do nothing */ }
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(spawnv_null); TC_RUN(spawnve_null); TC_RUN(spawnvp_null); TC_RUN(spawnvpe_null);
    TC_RUN(spawnv_empty_array);
    return tc_end();
}
