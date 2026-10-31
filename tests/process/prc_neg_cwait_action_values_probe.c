/* TC
 * id:    prc_neg_cwait_action_values_probe
 * area:  process
 * funcs: _cwait _spawnl
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait: "action ... ignored by Windows operating system applications"
 * note:  PROBE: with action = 77 (garbage) and _WAIT_GRANDCHILD the call must still succeed (return == handle) and report exit
 *        code 4 in termstat. Whether termstat holds the raw exit code (assumed here; some old CRTs may store it
 *        byte-swapped/shifted) is itself a probe; the return-value
 *        cases are separate so the termstat guess cannot mask them.
 */
#include "tc.h"
#include <process.h>

static char g_cmd[MAX_PATH + 16];
static intptr_t g_h, g_r; static int g_st;
static void go(int action)
{
    g_st = -1; g_r = -2;
    g_h = _spawnl(_P_NOWAIT, g_cmd, g_cmd, "/c", "exit 4", NULL);
    if (g_h == -1) return;
    g_r = _cwait(&g_st, g_h, action);
}
TC_CASE(ret_garbage)    { go(77); CHECK(g_h != -1); CHECK_EQ_I(g_r, g_h); }
TC_CASE(ret_grandchild) { go(_WAIT_GRANDCHILD); CHECK(g_h != -1); CHECK_EQ_I(g_r, g_h); }
TC_CASE(st_garbage)     { go(77); CHECK_EQ_I(g_st, 4); }
TC_CASE(st_grandchild)  { go(_WAIT_GRANDCHILD); CHECK_EQ_I(g_st, 4); }
int main(void)
{
    UINT n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    tc_begin();
    TC_RUN(ret_garbage); TC_RUN(ret_grandchild); TC_RUN(st_garbage); TC_RUN(st_grandchild);
    return tc_end();
}
