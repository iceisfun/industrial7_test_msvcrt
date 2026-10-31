/* TC
 * id:    prc_neg_spawnl_null_arg0_probe
 * area:  process
 * funcs: _spawnl _spawnle
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn (argv[0] must not be NULL; parameter validation)
 * note:  PROBES: _spawnl/_spawnle with a NULL first argument string (arg0) behave like argv[0]==NULL, i.e. return -1,
 *        errno EINVAL and call the invalid-parameter handler exactly once. The cmdname is a nonexistent path so that a CRT
 *        that does not validate arg0 fails with ENOENT instead of launching an interactive cmd.exe. For _spawnle the NULL arg0
 *        is itself the list terminator, so envp follows it directly.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char g_cmd[MAX_PATH + 16];
static wchar_t g_wcmd[MAX_PATH + 16];
static void init_paths(void)
{
    UINT n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    n = GetSystemDirectoryW(g_wcmd, MAX_PATH); lstrcpyW(g_wcmd + n, L"\\cmd.exe");
}

static const char *g_envp[2];

TC_CASE(spawnl_null_arg0)
{
    volatile int m = _P_WAIT; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _spawnl(m, "C:\\tc_no_such_dir\\x.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(spawnle_null_arg0)
{
    volatile int m = _P_WAIT; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _spawnle(m, "C:\\tc_no_such_dir\\x.exe", (char *)NULL, g_envp);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); init_paths();
    g_envp[0] = "TC_X=1"; g_envp[1] = NULL;
    TC_RUN(spawnl_null_arg0); TC_RUN(spawnle_null_arg0);
    return tc_end();
}
