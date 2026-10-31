/* TC
 * id:    prc_neg_spawn_nofile
 * area:  process
 * funcs: _spawnl _wspawnl _spawnv _spawnlp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn: returns -1 and sets errno (ENOENT if file not found)
 * note:  -
 */
#include "tc.h"
#include <process.h>
#include <errno.h>
#include <stdlib.h>

static char g_cmd[MAX_PATH + 16], g_self[MAX_PATH];
static wchar_t g_wcmd[MAX_PATH + 16], g_wself[MAX_PATH];
static void init_paths(void)
{
    UINT n = GetSystemDirectoryA(g_cmd, MAX_PATH); lstrcpyA(g_cmd + n, "\\cmd.exe");
    n = GetSystemDirectoryW(g_wcmd, MAX_PATH); lstrcpyW(g_wcmd + n, L"\\cmd.exe");
    GetModuleFileNameA(NULL, g_self, MAX_PATH); GetModuleFileNameW(NULL, g_wself, MAX_PATH);
}
TC_CASE(spawnl_enoent)
{
    errno = 0;
    CHECK_EQ_I(_spawnl(_P_WAIT, "C:\\tc_no_such_dir\\nope.exe", "nope.exe", NULL), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wspawnl_enoent)
{
    errno = 0;
    CHECK_EQ_I(_wspawnl(_P_WAIT, L"C:\\tc_no_such_dir\\nope.exe", L"nope.exe", NULL), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(spawnv_enoent)
{
    const char *av[] = { "nope.exe", NULL };
    errno = 0;
    CHECK_EQ_I(_spawnv(_P_WAIT, "C:\\tc_no_such_dir\\nope.exe", av), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(spawnlp_path_enoent)
{
    errno = 0;
    CHECK_EQ_I(_spawnlp(_P_WAIT, "tc_no_such_program_xyz.exe", "x", NULL), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(nowait_fail_no_handle)
{
    CHECK_EQ_I(_spawnl(_P_NOWAIT, "C:\\tc_no_such_dir\\nope.exe", "nope.exe", NULL), -1);
}
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(spawnl_enoent); TC_RUN(wspawnl_enoent); TC_RUN(spawnv_enoent); TC_RUN(spawnlp_path_enoent); TC_RUN(nowait_fail_no_handle);
    return tc_end();
}
