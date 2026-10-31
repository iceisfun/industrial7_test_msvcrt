/* TC
 * id:    prc_spawn_wait_exitcode
 * area:  process
 * funcs: _spawnl _spawnv _wspawnl
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions (_P_WAIT returns the exit status of the child)
 */
#include "tc.h"
#include <process.h>
#include <wchar.h>

static void cmdpath(char *p) { UINT n = GetSystemDirectoryA(p, MAX_PATH - 16); p[n] = 0; { const char *s = "\\cmd.exe"; int i = 0; while ((p[n + i] = s[i])) i++; } }

TC_CASE(self_exit_code) {
    char exe[MAX_PATH]; GetModuleFileNameA(NULL, exe, MAX_PATH);
    CHECK_EQ_I(_spawnl(_P_WAIT, exe, exe, "--tc-child=exit7", (char *)NULL), 7);
}
TC_CASE(cmd_spawnl) {
    char p[MAX_PATH]; cmdpath(p);
    CHECK_EQ_I(_spawnl(_P_WAIT, p, "cmd.exe", "/c", "exit", "9", (char *)NULL), 9);
}
TC_CASE(cmd_spawnv) {
    char p[MAX_PATH]; const char *av[5]; cmdpath(p);
    av[0] = "cmd.exe"; av[1] = "/c"; av[2] = "exit"; av[3] = "0"; av[4] = NULL;
    CHECK_EQ_I(_spawnv(_P_WAIT, p, av), 0);
    av[3] = "123";
    CHECK_EQ_I(_spawnv(_P_WAIT, p, av), 123);
}
TC_CASE(cmd_wspawnl) {
    char a[MAX_PATH]; wchar_t w[MAX_PATH]; int i;
    cmdpath(a); for (i = 0; (w[i] = (wchar_t)(unsigned char)a[i]); i++) ;
    CHECK_EQ_I(_wspawnl(_P_WAIT, w, L"cmd.exe", L"/c", L"exit", L"4", (wchar_t *)NULL), 4);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) _exit(7);
    TC_RUN(self_exit_code); TC_RUN(cmd_spawnl); TC_RUN(cmd_spawnv); TC_RUN(cmd_wspawnl);
    return tc_end();
}
