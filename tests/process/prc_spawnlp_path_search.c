/* TC
 * id:    prc_spawnlp_path_search
 * area:  process
 * funcs: _spawnlp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnlp: p variants search PATH
 * note:  Assumes %SystemRoot%\\system32 is on PATH.
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
TC_CASE(with_ext) { CHECK_EQ_I(_spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", "/c", "exit 4", NULL), 4); }
TC_CASE(no_ext)   { CHECK_EQ_I(_spawnlp(_P_WAIT, "cmd", "cmd", "/c", "exit 4", NULL), 4); }
int main(void)
{
    tc_begin(); init_paths();
    TC_RUN(with_ext); TC_RUN(no_ext);
    return tc_end();
}
