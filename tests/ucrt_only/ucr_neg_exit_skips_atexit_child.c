/* TC
 * id:    ucr_neg_exit_skips_atexit_child
 * area:  ucrt_only
 * kind:  negative
 * funcs: _exit exit
 * procs: _crt_atexit _Exit
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs exit, _exit, _Exit: "_exit and _Exit terminate without calling atexit/_onexit handlers"; C11 7.22.4.5
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *reg_t)(void (__cdecl *)(void));
static const char *mfile = "tc_ucr_neg_exit_skips.tmp";

static void __cdecl ha(void)
{
    DWORD n; char ch = 'X';
    HANDLE h = CreateFileA(mfile, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, &ch, 1, &n, NULL); CloseHandle(h);
}
static int marker_exists(void)
{
    return GetFileAttributesA(mfile) != INVALID_FILE_ATTRIBUTES;
}
static DWORD run(const char *name)
{
    DWORD code = 0; DeleteFileA(mfile);
    CHECK(tc_spawn_self(name, &code, 20000));
    return code;
}

TC_CASE(underscore_exit) { CHECK_EQ_U(run("ue"), 5); CHECK(!marker_exists()); DeleteFileA(mfile); }
TC_CASE(Exit_c99)        { CHECK_EQ_U(run("uE"), 6); CHECK(!marker_exists()); DeleteFileA(mfile); }
TC_CASE(exit_control)    { CHECK_EQ_U(run("ex"), 7); CHECK(marker_exists()); DeleteFileA(mfile); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        reg_t ar = (reg_t)tc_crt_proc("_crt_atexit");
        if (!ar || ar(ha) != 0) ExitProcess(90);
        if (c[0] == 'u' && c[1] == 'e') _exit(5);
        if (c[0] == 'u' && c[1] == 'E') ((void (__cdecl *)(int))tc_crt_proc("_Exit"))(6);
        if (c[0] == 'e') exit(7);
        return 77;
    }
    TC_RUN(underscore_exit); TC_RUN(Exit_c99); TC_RUN(exit_control);
    return tc_end();
}
