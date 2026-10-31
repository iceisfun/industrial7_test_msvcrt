/* TC
 * id:    gsp_neg_spawn_oldnames_bad_mode
 * area:  process
 * funcs: _spawnl _spawnv _spawnlp _spawnvp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _spawn functions: mode is one of _P_WAIT(0), _P_NOWAIT(1), _P_OVERLAY(2), _P_NOWAITO(3), _P_DETACH(4); Parameter validation
 * note:  Recursion guard: named mutex Local\tc_gsp_norec_mode_<dll> held by the real run makes an accidentally spawned copy exit 0 at once.
 *        Modes -1 and 99 are outside the defined set. The docs do not spell out the failure result; EINVAL + handler is the
 *        general parameter-validation contract. The program is this exe, so a bad CRT that ignored mode would start a child.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_self[MAX_PATH];
static char *g_argv[] = { (char *)"x", NULL };
static volatile int g_mode;

static void pre(void) { if (!tc_iph_install()) return; tc_iph_count = 0; errno = 0; }
static void post(intptr_t r)
{
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(spawnl_m1)  { g_mode = -1; pre(); post(spawnl(g_mode, g_self, g_self, (char *)NULL)); }
TC_CASE(spawnl_99)  { g_mode = 99; pre(); post(spawnl(g_mode, g_self, g_self, (char *)NULL)); }
TC_CASE(spawnv_m1)  { g_mode = -1; pre(); post(spawnv(g_mode, g_self, g_argv)); }
TC_CASE(spawnv_99)  { g_mode = 99; pre(); post(spawnv(g_mode, g_self, g_argv)); }
TC_CASE(spawnlp_99) { g_mode = 99; pre(); post(spawnlp(g_mode, g_self, g_self, (char *)NULL)); }
TC_CASE(spawnvp_m1) { g_mode = -1; pre(); post(spawnvp(g_mode, g_self, g_argv)); }
int main(void)
{
    tc_begin();
    { HANDLE gh = CreateMutexA(NULL, FALSE, "Local\\tc_gsp_norec_mode_" TC_CRT_DLL);
      if (gh && GetLastError() == ERROR_ALREADY_EXISTS) return 0;   /* accidentally spawned copy of this exe: do nothing */ }
    GetModuleFileNameA(NULL, g_self, MAX_PATH);
    TC_RUN(spawnl_m1); TC_RUN(spawnl_99); TC_RUN(spawnv_m1); TC_RUN(spawnv_99);
    TC_RUN(spawnlp_99); TC_RUN(spawnvp_m1);
    return tc_end();
}
