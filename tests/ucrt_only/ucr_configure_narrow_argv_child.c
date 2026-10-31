/* TC
 * id:    ucr_configure_narrow_argv_child
 * area:  ucrt_only
 * funcs:
 * procs: _configure_narrow_argv __p___argc __p___argv
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _configure_narrow_argv (corecrt_startup.h); _crt_argv_mode
 * note:  PROBE: re-calling _configure_narrow_argv after startup (in a child, to isolate the re-parse) returns 0 and
 *        rebuilds argc/argv. Mode 1 = _crt_argv_unexpanded_arguments, 2 = _crt_argv_expanded_arguments.
 *        Child is launched as: exe --tc-child=<name> *.tc_nonexistent_zz
 */
#include "tc.h"

typedef int    (__cdecl *cfg_t)(int);
typedef int *  (__cdecl *pargc_t)(void);
typedef char***(__cdecl *pargv_t)(void);
static cfg_t cfg; static pargc_t pargc; static pargv_t pargv;

static int child_main(const char *n)
{
    int mode = n[0] == 'e' ? 2 : 1, i, found = 0;
    if (!cfg || !pargc || !pargv) return 70;
    if (cfg(mode) != 0) return 71;
    if (*pargc() < 1) return 72;
    if (!(*pargv())[0]) return 73;
    for (i = 0; i < *pargc(); i++) if (tc_strequ((*pargv())[i], "*.tc_nonexistent_zz")) found = 1;
    if (!found) return 74;
    return 0;
}
static void run(const char *name)
{
    DWORD code = 99; char cmd[MAX_PATH + 128], exe[MAX_PATH]; STARTUPINFOA si; PROCESS_INFORMATION pi; int i, j = 0;
    const char *tail = " --tc-child=";
    const char *arg = " *.tc_nonexistent_zz";
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    cmd[j++] = '"'; for (i = 0; exe[i]; i++) cmd[j++] = exe[i]; cmd[j++] = '"';
    for (i = 0; tail[i]; i++) cmd[j++] = tail[i];
    for (i = 0; name[i]; i++) cmd[j++] = name[i];
    for (i = 0; arg[i]; i++) cmd[j++] = arg[i];
    cmd[j] = 0;
    ZeroMemory(&si, sizeof si); si.cb = sizeof si;
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) { CHECK(!"CreateProcess"); return; }
    WaitForSingleObject(pi.hProcess, 20000);
    GetExitCodeProcess(pi.hProcess, &code); CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    CHECK_EQ_U(code, 0);
}
TC_CASE(exported) { CHECK(cfg != NULL); CHECK(pargc != NULL); CHECK(pargv != NULL); }
TC_CASE(unexpanded) { run("unexpanded"); }
TC_CASE(expanded)   { run("expanded"); }
int main(void)
{
    const char *c;
    tc_begin();
    cfg = (cfg_t)tc_crt_proc("_configure_narrow_argv");
    pargc = (pargc_t)tc_crt_proc("__p___argc");
    pargv = (pargv_t)tc_crt_proc("__p___argv");
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(exported); TC_RUN(unexpanded); TC_RUN(expanded);
    return tc_end();
}
