/* TC
 * id:    ext_probe_neg_set_abort_behavior_msg_only
 * area:  exit_signal
 * funcs: abort
 * procs: _set_abort_behavior _set_error_mode
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_abort_behavior, abort
 * note:  PROBES: with _WRITE_ABORT_MSG on and _CALL_REPORTFAULT off, abort() exits with 3 and writes some message to
 *        stderr (child's STD_ERROR_HANDLE redirected to a temp file via SetStdHandle). Message content not asserted.
 *        Exit code and file size are separate cases.
 */
#include "tc.h"
#include <stdlib.h>

#define FN "tc_ext_probe_abort_msg.tmp"
typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
static DWORD code = 0xdeadbeef;
static int ran;

static void go(void)
{
    if (ran) return;
    ran = 1; DeleteFileA(FN);
    if (!tc_spawn_self("msg", &code, 20000)) code = 0xdead0002;
}
TC_CASE(exit_code_3) { go(); CHECK_EQ_U(code, 3); }
TC_CASE(stderr_nonempty)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    go();
    CHECK(GetFileAttributesExA(FN, GetFileExInfoStandard, &d));
    CHECK(d.nFileSizeLow > 0 || d.nFileSizeHigh > 0);
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        HANDLE h;
        int (__cdecl *sem)(int) = (int (__cdecl *)(int))tc_crt_proc("_set_error_mode");
        if (!sab) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        if (sem) sem(1);   /* _OUT_TO_STDERR: avoid a message box */
        h = CreateFileA(FN, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE) return 79;
        SetStdHandle(STD_ERROR_HANDLE, h);
        sab(0x1, 0x1 | 0x2);   /* _WRITE_ABORT_MSG on, _CALL_REPORTFAULT off */
        abort();
        return 77;
    }
    TC_RUN(exit_code_3); TC_RUN(stderr_nonempty);
    return tc_end();
}
