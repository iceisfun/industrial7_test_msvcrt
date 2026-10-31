/* TC
 * id:    ucr_cexit_vs_c_exit_child
 * area:  ucrt_only
 * funcs: _cexit _c_exit
 * procs: _crt_atexit atexit
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _cexit, _c_exit: _cexit calls atexit/_onexit functions and flushes; _c_exit does neither; neither terminates the process
 */
#include "tc.h"
#include <process.h>

typedef int (__cdecl *reg_t)(void (__cdecl *)(void));
static const char *mfile = "tc_ucr_cexit.tmp";

static void __cdecl ha(void)
{
    DWORD n; char ch = 'X';
    HANDLE h = CreateFileA(mfile, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static int marker_exists(void) { return GetFileAttributesA(mfile) != INVALID_FILE_ATTRIBUTES; }
static DWORD run(const char *name)
{
    DWORD code = 0; DeleteFileA(mfile);
    CHECK(tc_spawn_self(name, &code, 20000));
    return code;
}

TC_CASE(cexit_runs_handlers)   { CHECK_EQ_U(run("cx"), 21); CHECK(marker_exists()); DeleteFileA(mfile); }
TC_CASE(c_exit_skips_handlers) { CHECK_EQ_U(run("cc"), 22); CHECK(!marker_exists()); DeleteFileA(mfile); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
#if TC_AT_LEAST(140)
        reg_t ar = (reg_t)tc_crt_proc("_crt_atexit");
#else
        reg_t ar = (reg_t)tc_crt_proc("atexit");
#endif
        if (!ar || ar(ha) != 0) ExitProcess(90);
        if (c[1] == 'x') { _cexit(); ExitProcess(21); }
        _c_exit();
        /* TerminateProcess: avoid any CRT-DLL detach-time termination running handlers after _c_exit */
        TerminateProcess(GetCurrentProcess(), 22);
    }
    TC_RUN(cexit_runs_handlers); TC_RUN(c_exit_skips_handlers);
    return tc_end();
}
